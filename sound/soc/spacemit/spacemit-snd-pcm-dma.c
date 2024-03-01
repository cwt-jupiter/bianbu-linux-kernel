// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2022 SPACEMIT Micro Limited
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/dmaengine_pcm.h>
#include <linux/genalloc.h>

#define DRV_NAME "spacemit-snd-dma"

#define I2S0_REG_BASE            0xD4026000
#define I2S1_REG_BASE            0xD4026800
#define DATAR                    0x10    /* SSP Data Register */

#define I2S_HDMI_REG_BASE        0xC0883900

#define HDMI_REFORMAT_ENABLE

#define L_CH                            0
#define R_CH                            1
#define SAMPLE_PRESENT_FLAG_OFFSET      31
#define AUDIO_FRAME_START_BIT_OFFSET    30
#define SAMPLE_FLAT_BIT_OFFSET          29
#define NOT_USED_OFFSET                 28
#define PARITY_BIT_OFFSET               27
#define CHANNEL_STATUS_OFFSET           26
#define USER_DATA_OFFSET                25
#define VALID_OFFSET                    24

#define IEC958_AES0_CONSUMER            (0<<0)	/* 0 = consumer, 1 = professional */
#define IEC958_AES0_AUDIO               (0<<1)	/* 0 = audio, 1 = non-audio */
#define IEC958_AES3_CON_FS_48000        (2<<0)	/* 48kHz */
#define IEC958_AES0_CON_NOT_COPYRIGHT   (1<<2)	/* 0 = copyright, 1 = not copyright */
#define IEC958_AES0_CON_EMPHASIS_NONE   (0<<3)	/* none emphasis */
#define IEC958_AES1_CON_GENERAL         (0x00)
#define IEC958_AES2_CON_SOURCE_UNSPEC   (0<<0)	/* unspecified */
#define IEC958_AES2_CON_CHANNEL_UNSPEC  (0<<4)	/* unspecified */
#define IEC958_AES3_CON_CLOCK_1000PPM   (0<<4)	/* 1000 ppm */
#define IEC958_AES4_CON_WORDLEN_24_20   (5<<1)	/* 24-bit or 20-bit */
#define IEC958_AES4_CON_MAX_WORDLEN_24  (1<<0)	/* 0 = 20-bit, 1 = 24-bit */

#define CS_CTRL1 ((1 << SAMPLE_PRESENT_FLAG_OFFSET) | (1 << AUDIO_FRAME_START_BIT_OFFSET))
#define CS_CTRL2 ((1 << SAMPLE_PRESENT_FLAG_OFFSET) | (0 << AUDIO_FRAME_START_BIT_OFFSET))

#define P2(n) n, n^1, n^1, n
#define P4(n) P2(n), P2(n^1), P2(n^1), P2(n)
#define P6(n) P4(n), P4(n^1), P4(n^1), P4(n)

struct spacemit_snd_dmadata {
	struct dma_chan *dma_chan;
	unsigned int dma_id;
	dma_cookie_t cookie;
	spinlock_t dma_lock;
	void *private_data;

	int stream;
	/*DOMAIN:config from userspace*/
	struct snd_pcm_substream *substream;
	unsigned long pos;
};

struct spacemit_snd_soc_device {
	struct spacemit_snd_dmadata dmadata[2];
	unsigned long pos;
};

struct hdmi_priv {
	struct gen_pool *pool;
	struct resource *buf_res;
	dma_addr_t phy_addr;
	void __iomem	*buf_base;

};

/* HDMI initalization data */
struct hdmi_codec_priv {
    uint32_t srate;
    uint32_t channels;
    uint8_t iec_offset;
    uint8_t ch_sn;
    uint8_t cs[24];
};

struct hdmi_codec_priv hdmi_ptr = {0};
static const bool ParityTable256[256] =
{
    P6(0), P6(1), P6(1), P6(0)
};
static struct hdmi_priv priv;
static dma_addr_t hdmiraw_dma_addr;
static dma_addr_t hdmipcm_dma_addr;
static unsigned char *hdmiraw_dma_area;	/* DMA area */
#ifdef HDMI_REFORMAT_ENABLE
static unsigned char *hdmiraw_dma_area_tmp;
#endif

static int spacemit_snd_dma_init(struct device *paraent, struct spacemit_snd_soc_device *dev,
			     struct spacemit_snd_dmadata *dmadata);

static const struct snd_pcm_hardware spacemit_snd_pcm_hardware = {

	.info		  = SNDRV_PCM_INFO_INTERLEAVED |
			    SNDRV_PCM_INFO_BATCH |
			    SNDRV_PCM_INFO_PAUSE,
	.buffer_bytes_max = 64 * 1024,
	.period_bytes_min = 32,
	.period_bytes_max = 8 * 1024,
	.periods_min	  = 2,
	.periods_max	  = 32,
};

static const struct snd_pcm_hardware spacemit_snd_pcm_hardware_hdmi = {

	.info		  = SNDRV_PCM_INFO_INTERLEAVED |
			    SNDRV_PCM_INFO_BATCH |
			    SNDRV_PCM_INFO_PAUSE,
	.formats	  = SNDRV_PCM_FMTBIT_S16_LE,
	.rates		  = SNDRV_PCM_RATE_48000,
	.rate_min	  = SNDRV_PCM_RATE_48000,
	.rate_max	  = SNDRV_PCM_RATE_48000,
	.channels_min	  = 2,
	.channels_max	  = 2,
	.buffer_bytes_max = 256 * 4 * 4,
	.period_bytes_min = 256 * 4,
	.period_bytes_max = 256 * 4,
	.periods_min	  = 4,
	.periods_max	  = 4,
};
static int spacemit_dma_slave_config(struct snd_pcm_substream *substream,
		struct snd_pcm_hw_params *params,
		struct dma_slave_config *slave_config,
		int dma_id)
{
	int ret = snd_hwparams_to_dma_slave_config(substream, params, slave_config);
	if (ret)
		return ret;

	slave_config->dst_maxburst = 32;
	slave_config->src_maxburst = 32;

	slave_config->src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	slave_config->dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	/* for tmda , don't need to config dma controller addr, set to 0*/
	if (dma_id == 0) {
		pr_debug("i2s0_datar\n");
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			slave_config->dst_addr = I2S0_REG_BASE + DATAR;
			slave_config->src_addr = 0;
		}
		else {
			slave_config->src_addr = I2S0_REG_BASE + DATAR;
			slave_config->dst_addr = 0;
		}
	} else if (dma_id == 1) {
		pr_debug("i2s1_datar\n");
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			slave_config->dst_addr = I2S1_REG_BASE + DATAR;
			slave_config->src_addr = 0;
		}
		else {
			slave_config->src_addr = I2S1_REG_BASE + DATAR;
			slave_config->dst_addr = 0;
		}
	} else if (dma_id == 2) {
		pr_debug("i2s_hdmi_datar\n");
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
			slave_config->dst_addr = I2S_HDMI_REG_BASE + 0x80;
			slave_config->src_addr = 0;
			#ifdef HDMI_REFORMAT_ENABLE
			slave_config->src_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
			slave_config->dst_addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
			#else
			slave_config->src_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
			slave_config->dst_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
			#endif
		}
		else {
			slave_config->src_addr = I2S_HDMI_REG_BASE + 0x00;
			slave_config->dst_addr = 0;
		}
	}else {
		pr_err("unsupport dma platform\n");
		return -1;
	}

	pr_debug("leave %s\n", __FUNCTION__);

	return 0;
}

static int spacemit_snd_pcm_hw_params(struct snd_soc_component *component, struct snd_pcm_substream *substream,
				  struct snd_pcm_hw_params *params)
{
	int ret;
	int dma_id;
	struct dma_slave_config slave_config;

	struct snd_pcm_runtime *runtime = substream->runtime;
	struct spacemit_snd_dmadata *dmadata = runtime->private_data;
	const char *name = dev_name(component->dev);

	pr_debug("enter %s!! allocbytes=%d, dmadata=0x%lx\n",
		__FUNCTION__, params_buffer_bytes(params), (unsigned long)dmadata);

	memset(&slave_config, 0, sizeof(slave_config));
	if (!strcmp(name, "spacemit-snd-dma0")) {
		dma_id = 0;
		dmadata->dma_id = dma_id;
	} else if (!strcmp(name, "spacemit-snd-dma1")) {
		dma_id = 1;
		dmadata->dma_id = dma_id;
	} else {
		pr_err("unsupport dma platform\n");
		return -1;
	}
	spacemit_dma_slave_config(substream, params, &slave_config, dma_id);

	ret = dmaengine_slave_config(dmadata->dma_chan, &slave_config);
	if (ret)
		return ret;

	ret = snd_pcm_lib_malloc_pages(substream, params_buffer_bytes(params));
	if (ret < 0)
		return ret;

	dmadata->substream = substream;
	dmadata->pos = 0;

	pr_debug("leave %s!!\n", __FUNCTION__);
	return 0;
}

static int spacemit_snd_pcm_hw_free(struct snd_soc_component *component, struct snd_pcm_substream *substream)
{
	pr_debug("enter %s!!\n", __FUNCTION__);
	return snd_pcm_lib_free_pages(substream);
}

static int spacemit_snd_pcm_hdmi_hw_params(struct snd_soc_component *component, struct snd_pcm_substream *substream,
				  struct snd_pcm_hw_params *params)
{
	//config hdmi and callback
	int ret;
	int dma_id;
	struct dma_slave_config slave_config;

	struct snd_pcm_runtime *runtime = substream->runtime;
	struct spacemit_snd_dmadata *dmadata = runtime->private_data;
	const char *name = dev_name(component->dev);

	pr_debug("enter %s!! allocbytes=%d, dmadata=0x%lx\n",
		__FUNCTION__, params_buffer_bytes(params), (unsigned long)dmadata);

	memset(&slave_config, 0, sizeof(slave_config));
	if (!strcmp(name, "c08d0400.spacemit-snd-dma-hdmi")) {
		dma_id = 2;
		dmadata->dma_id = dma_id;
	} else {
		pr_err("unsupport dma platform\n");
		return -1;
	}
	spacemit_dma_slave_config(substream, params, &slave_config, dma_id);

	ret = dmaengine_slave_config(dmadata->dma_chan, &slave_config);
	if (ret)
		return ret;

	ret = snd_pcm_lib_malloc_pages(substream, params_buffer_bytes(params));
	if (ret < 0)
		return ret;

	hdmiraw_dma_area = (void *)priv.buf_base;
	hdmiraw_dma_addr = (dma_addr_t)priv.phy_addr;

	if (hdmiraw_dma_area == NULL) {
		pr_err("hdmi:raw:get mem failed...\n");
		return -ENOMEM;
	}
#ifdef HDMI_REFORMAT_ENABLE
	hdmiraw_dma_area_tmp = kzalloc((params_buffer_bytes(params) * 2), GFP_KERNEL);
	if (hdmiraw_dma_area_tmp == NULL) {
		pr_err("hdmi:raw:get mem tmp failed...\n");
		return -ENOMEM;
	}
#endif

	hdmipcm_dma_addr = substream->dma_buffer.addr;
	substream->dma_buffer.addr = (dma_addr_t)hdmiraw_dma_addr;

	dmadata->substream = substream;
	dmadata->pos = 0;
	snd_pcm_set_runtime_buffer(substream, &substream->dma_buffer);

	pr_debug("leave %s!!\n", __FUNCTION__);
	return 0;
}

static int spacemit_snd_pcm_hdmi_hw_free(struct snd_soc_component *component, struct snd_pcm_substream *substream)
{
	pr_debug("enter %s!!\n", __FUNCTION__);

	substream->dma_buffer.addr = hdmipcm_dma_addr;
	hdmiraw_dma_area = NULL;
#ifdef HDMI_REFORMAT_ENABLE
	kfree(hdmiraw_dma_area_tmp);
#endif
	return snd_pcm_lib_free_pages(substream);
}
static void spacemit_snd_dma_complete(void *arg)
{
	struct spacemit_snd_dmadata *dmadata = arg;
	struct snd_pcm_substream *substream = dmadata->substream;
	unsigned int pos = dmadata->pos;

	pr_debug("enter %s\n", __FUNCTION__);

	pos += snd_pcm_lib_period_bytes(substream);
	pos %= snd_pcm_lib_buffer_bytes(substream);
	dmadata->pos = pos;

	if (snd_pcm_running(substream)) {
		snd_pcm_period_elapsed(substream);
	}
}

static int spacemit_snd_dma_submit(struct spacemit_snd_dmadata *dmadata)
{
	struct dma_async_tx_descriptor *desc;
	struct snd_pcm_substream *substream = dmadata->substream;
	struct dma_chan *chan = dmadata->dma_chan;
	unsigned long flags = DMA_CTRL_ACK;

	pr_debug("enter %s!!\n", __FUNCTION__);

	if (!substream->runtime->no_period_wakeup)
		flags |= DMA_PREP_INTERRUPT;

#ifdef HDMI_REFORMAT_ENABLE
	if (dmadata->dma_id == 2){
		desc = dmaengine_prep_dma_cyclic(chan,
			substream->runtime->dma_addr,
			snd_pcm_lib_buffer_bytes(substream) * 2,
			snd_pcm_lib_period_bytes(substream) * 2,
			snd_pcm_substream_to_dma_direction(substream), 
			flags);
	} else
#endif
	{
		desc = dmaengine_prep_dma_cyclic(chan,
			substream->runtime->dma_addr,
			snd_pcm_lib_buffer_bytes(substream),
			snd_pcm_lib_period_bytes(substream),
			snd_pcm_substream_to_dma_direction(substream), 
			flags);
	}

	if (!desc)
		return -ENOMEM;

	desc->callback = spacemit_snd_dma_complete;
	desc->callback_param = dmadata;
	dmadata->cookie = dmaengine_submit(desc);

	return 0;
}

static int spacemit_snd_pcm_trigger(struct snd_soc_component *component, struct snd_pcm_substream *substream, int cmd)
{
	struct spacemit_snd_dmadata *dmadata = substream->runtime->private_data;
	struct snd_pcm_runtime *runtime = substream->runtime;

	int ret = 0;
	pr_debug("pcm_trigger: cmd=%d,dir=%d\n", cmd, substream->stream);
	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
		ret = spacemit_snd_dma_submit(dmadata);
		if (ret < 0)
			return ret;
		dma_async_issue_pending(dmadata->dma_chan);
		break;
	case SNDRV_PCM_TRIGGER_STOP:
		dmaengine_terminate_async(dmadata->dma_chan);
		dmadata->pos = 0;
		break;
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		dmaengine_pause(dmadata->dma_chan);
		break;
	case SNDRV_PCM_TRIGGER_SUSPEND:
		if (runtime->info & SNDRV_PCM_INFO_PAUSE)
			dmaengine_pause(dmadata->dma_chan);
		else
			dmaengine_terminate_async(dmadata->dma_chan);
		break;
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
	case SNDRV_PCM_TRIGGER_RESUME:
		dmaengine_resume(dmadata->dma_chan);
		break;
	default:
		ret = -EINVAL;
	}
	return ret;
}

static snd_pcm_uframes_t
spacemit_snd_pcm_pointer(struct snd_soc_component *component, struct snd_pcm_substream *substream)
{
	struct spacemit_snd_dmadata *dmadata = substream->runtime->private_data;
	return bytes_to_frames(substream->runtime, dmadata->pos);
}
static void hdmi_create_cs(struct hdmi_codec_priv *hdmi_priv)
{
    uint8_t *cs;
    memset(hdmi_priv->cs, 0, sizeof(hdmi_priv->cs));
    cs = hdmi_priv->cs;
    cs[0] = IEC958_AES0_CONSUMER | IEC958_AES0_AUDIO | IEC958_AES0_CON_NOT_COPYRIGHT | IEC958_AES0_CON_EMPHASIS_NONE;
    cs[1] = IEC958_AES1_CON_GENERAL;
    cs[2] = IEC958_AES2_CON_SOURCE_UNSPEC | IEC958_AES2_CON_CHANNEL_UNSPEC;
    cs[3] = IEC958_AES3_CON_CLOCK_1000PPM | IEC958_AES3_CON_FS_48000;
    cs[4] = IEC958_AES4_CON_WORDLEN_24_20 | IEC958_AES4_CON_MAX_WORDLEN_24;		//24bits

}
static int spacemit_snd_pcm_open(struct snd_soc_component *component, struct snd_pcm_substream *substream)
{
	int ret;
	struct spacemit_snd_soc_device *dev;
	struct spacemit_snd_dmadata *dmadata;
	struct snd_soc_pcm_runtime *rtd = snd_pcm_substream_chip(substream);
	const char *name = dev_name(component->dev);

	pr_debug("%s enter, rtd->dev=%s,dir=%d\n", __FUNCTION__, dev_name(rtd->dev),substream->stream);

	if (!component) {
		pr_err("%s!! coundn't find component %s\n", __FUNCTION__, DRV_NAME);
		return -1;
	}

	dev = snd_soc_component_get_drvdata(component);
	if (!dev) {
		pr_err("%s!! get dev error\n", __FUNCTION__);
		return -1;
	}

	dmadata = &dev->dmadata[substream->stream];
	if (!strcmp(name, "c08d0400.spacemit-snd-dma-hdmi")) {
		ret = snd_soc_set_runtime_hwparams(substream, &spacemit_snd_pcm_hardware_hdmi);
	} else {
		ret = snd_soc_set_runtime_hwparams(substream, &spacemit_snd_pcm_hardware);
	}

	if (ret)
		return ret;

	ret = snd_pcm_hw_constraint_integer(substream->runtime, SNDRV_PCM_HW_PARAM_PERIODS);
	if (ret < 0) {
		pr_err("%s!! got serror\n", __FUNCTION__);
		return ret;
	}
	substream->runtime->private_data = dmadata;

	if (!strcmp(name, "c08d0400.spacemit-snd-dma-hdmi")) {
		hdmi_ptr.ch_sn = L_CH;
        hdmi_ptr.iec_offset = 0;
        hdmi_ptr.srate = 48000;
        hdmi_create_cs(&hdmi_ptr);
	}
	pr_debug("%s exit\n", __FUNCTION__);

	return 0;
}

static int spacemit_snd_pcm_close(struct snd_soc_component *component, struct snd_pcm_substream *substream)
{
	struct spacemit_snd_dmadata *dmadata = substream->runtime->private_data;
	struct dma_chan *chan = dmadata->dma_chan;
	pr_debug("%s debug, dir=%d\n", __FUNCTION__,substream->stream);
	dmaengine_terminate_all(chan);
	hdmi_ptr.ch_sn = L_CH;
    hdmi_ptr.iec_offset = 0;

	return 0;
}

static int spacemit_snd_pcm_lib_ioctl(struct snd_soc_component *component, struct snd_pcm_substream *substream,
		      unsigned int cmd, void *arg)
{
	return snd_pcm_lib_ioctl(substream, cmd, arg);
}

static const char * const spacemit_pcm_dma_channel_names[] = {
	[SNDRV_PCM_STREAM_PLAYBACK] = "tx",
	[SNDRV_PCM_STREAM_CAPTURE] = "rx",
};

static int spacemit_snd_pcm_new(struct snd_soc_component *component, struct snd_soc_pcm_runtime *rtd)
{
	int i;
	int ret;
	int chan_num;

	struct spacemit_snd_soc_device *dev;
	struct snd_card *card = rtd->card->snd_card;
	struct snd_pcm *pcm = rtd->pcm;

	printk("%s enter, dev=%s\n", __FUNCTION__, dev_name(rtd->dev));

	if (!component) {
		pr_err("%s: coundn't find component %s\n", __FUNCTION__, DRV_NAME);
		return -1;
	}

	dev = snd_soc_component_get_drvdata(component);
	if (!dev) {
		pr_err("%s: get dev error\n", __FUNCTION__);
		return -1;
	}
	if (!strcmp(dev_name(rtd->dev), "ADSP SSPA2 PCM")) {
		chan_num = 1;
		printk("%s playback_only, dev=%s\n", __FUNCTION__, dev_name(rtd->dev));
	}else{
		chan_num = 2;
	}
	dev->dmadata[0].stream = SNDRV_PCM_STREAM_PLAYBACK;
	dev->dmadata[1].stream = SNDRV_PCM_STREAM_CAPTURE;

	for (i = 0; i < chan_num; i++) {
		ret = spacemit_snd_dma_init(component->dev, dev, &dev->dmadata[i]);
		if (ret)
			goto exit;
	}

	snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,
		card->dev, 64 * 1024 * 32, 4 * 1024 * 1024);

	return 0;

exit:
	for (i = 0; i < chan_num; i++) {
		if (dev->dmadata[i].dma_chan)
			dma_release_channel(dev->dmadata[i].dma_chan);
		dev->dmadata[i].dma_chan = NULL;
	}

	return ret;
}

static int spacemit_snd_dma_init(struct device *paraent, struct spacemit_snd_soc_device *dev,
			     struct spacemit_snd_dmadata *dmadata)
{
	dma_cap_mask_t mask;
	spin_lock_init(&dmadata->dma_lock);

	dma_cap_zero(mask);
	dma_cap_set(DMA_SLAVE, mask);
	dma_cap_set(DMA_CYCLIC, mask);

	dmadata->dma_chan = dma_request_slave_channel(paraent, spacemit_pcm_dma_channel_names[dmadata->stream]);
	if (!dmadata->dma_chan) {
		pr_err("DMA channel for %s is not available\n",
			dmadata->stream == SNDRV_PCM_STREAM_PLAYBACK ?
			"playback" : "capture");
		return -EBUSY;
	}

	return 0;
}

static int spacemit_snd_pcm_probe(struct snd_soc_component *component)
{
	const char *name = dev_name(component->dev);
	struct spacemit_snd_soc_device *spacemit_snd_device = kzalloc(sizeof(struct spacemit_snd_soc_device), GFP_KERNEL);
	if (!spacemit_snd_device) {
		pr_err("%s: alloc memoery failed\n", __FUNCTION__);
		return -ENOMEM;
	}

	pr_info("%s enter: dev=%s\n", __FUNCTION__, name);

	snd_soc_component_set_drvdata(component, spacemit_snd_device);

	spacemit_snd_device->dmadata[0].private_data = spacemit_snd_device;
	spacemit_snd_device->dmadata[1].private_data = spacemit_snd_device;

	return 0;
}

static void spacemit_snd_pcm_remove(struct snd_soc_component *component)
{
	int i;
	struct spacemit_snd_soc_device *dev = snd_soc_component_get_drvdata(component);

	pr_info("%s enter\n", __FUNCTION__);

	for (i = 0; i < 2; i++) {
		struct spacemit_snd_dmadata *dmadata = &dev->dmadata[i];
		struct dma_chan *chan = dmadata->dma_chan;

		if (chan) {
			dmaengine_terminate_all(chan);
			dma_release_channel(chan);
		}
		dev->dmadata[i].dma_chan = NULL;
	}

	kfree(dev);
}
static void hdmi_set_cs_channel_sn(struct hdmi_codec_priv *hdmi_priv)
{
    hdmi_priv->cs[2] &= 0x0f;
    if (hdmi_priv->ch_sn == L_CH) {
        hdmi_priv->cs[2] |= (0x1 << 4);
        hdmi_priv->ch_sn = R_CH;
    } else if (hdmi_priv->ch_sn == R_CH)  {
        hdmi_priv->cs[2] |= (0x2 << 4);
        hdmi_priv->ch_sn = L_CH;
    }
}

static uint32_t get_cs_bit(struct hdmi_codec_priv *hdmi_priv)
{
    unsigned long tmp = 0;
    int cs_idx;
    int bit_idx;
    cs_idx = hdmi_priv->iec_offset >> 3;
    bit_idx = hdmi_priv->iec_offset - (cs_idx << 3);

    tmp = hdmi_priv->cs[cs_idx]>> bit_idx;

    return (uint32_t)tmp&0x1;
}

static uint32_t parity_even(uint32_t sample)
{
	bool parity = 0;
    sample ^= sample >> 16;
    sample ^= sample >> 8;
    parity = ParityTable256[sample & 0xff];
    if (parity)
        return 1;
    else
        return 0;
}

static void hdmi_reformat(void *dst, void *src, int len)
{

    uint32_t *dst32 = (uint32_t *)dst;
    uint16_t *src16 = (uint16_t *)src;
    struct hdmi_codec_priv *dw = &hdmi_ptr;
    uint16_t frm_cnt = len;
    uint32_t ctrl;
    uint32_t sample,parity;
    dw->channels = 2;
    while (frm_cnt--) {
        for (int i = 0; i < dw->channels; i++) {
            hdmi_set_cs_channel_sn(dw);
            if (dw->iec_offset == 0) {
                ctrl = CS_CTRL1 | (get_cs_bit(dw) << CHANNEL_STATUS_OFFSET);
            }  else {
                ctrl = CS_CTRL2 | (get_cs_bit(dw) << CHANNEL_STATUS_OFFSET);
            }

            sample = ((uint32_t)(*src16++) << 8)| ctrl;
            parity = parity_even(sample);
            sample = sample | (parity << PARITY_BIT_OFFSET);
            *dst32++ = sample;
        }

        dw->iec_offset++;
        if (dw->iec_offset >= 192){
            dw->iec_offset = 0;
		}
    };
}

static int spacemit_snd_pcm_copy(struct snd_soc_component *component, struct snd_pcm_substream *substream, int channel, 
unsigned long hwoff, void __user *buf, unsigned long bytes){
	int ret = 0;
	char *hwbuf;
	char *hdmihw_area;
	struct snd_pcm_runtime *runtime = substream->runtime;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
#ifdef HDMI_REFORMAT_ENABLE
		hwbuf = runtime->dma_area + hwoff;
		if (copy_from_user(hwbuf, buf, bytes))
			return -EFAULT;
		hdmihw_area = hdmiraw_dma_area_tmp + 2 * hwoff;
		hdmi_reformat((int *)hdmihw_area, (short *)hwbuf, bytes_to_frames(substream->runtime, bytes));
		memcpy((void *)(hdmiraw_dma_area + 2 * hwoff), (void *)hdmihw_area, bytes * 2);
#else
		hwbuf = hdmiraw_dma_area + hwoff;
		if (hwbuf == NULL)
			pr_err("%s addr null !!!!!!!!!!!!\n", __func__);
		if (copy_from_user(hwbuf, buf, bytes))
			return -EFAULT;
#endif

	} else if (substream->stream == SNDRV_PCM_STREAM_CAPTURE) {
		hwbuf = runtime->dma_area + hwoff;
		if (copy_to_user(buf, hwbuf, bytes))
			return -EFAULT;
	}

	return ret;
}
static const struct snd_soc_component_driver spacemit_snd_dma_component = {
	.name          = DRV_NAME,
	.probe		   = spacemit_snd_pcm_probe,
	.remove		   = spacemit_snd_pcm_remove,
	.open		   = spacemit_snd_pcm_open,
	.close		   = spacemit_snd_pcm_close,
	.ioctl		   = spacemit_snd_pcm_lib_ioctl,
	.hw_params	   = spacemit_snd_pcm_hw_params,
	.hw_free	   = spacemit_snd_pcm_hw_free,
	.trigger	   = spacemit_snd_pcm_trigger,
	.pointer	   = spacemit_snd_pcm_pointer,
	.pcm_construct = spacemit_snd_pcm_new
};

static const struct snd_soc_component_driver spacemit_snd_dma_component_hdmi = {
	.name          = DRV_NAME,
	.probe		   = spacemit_snd_pcm_probe,
	.remove		   = spacemit_snd_pcm_remove,
	.open		   = spacemit_snd_pcm_open,
	.close		   = spacemit_snd_pcm_close,
	.ioctl		   = spacemit_snd_pcm_lib_ioctl,
	.hw_params	   = spacemit_snd_pcm_hdmi_hw_params,
	.hw_free	   = spacemit_snd_pcm_hdmi_hw_free,
	.trigger	   = spacemit_snd_pcm_trigger,
	.pointer	   = spacemit_snd_pcm_pointer,
	.pcm_construct = spacemit_snd_pcm_new,
	.copy_user		= spacemit_snd_pcm_copy,
};

static int spacemit_snd_dma_pdev_probe(struct platform_device *pdev)
{
	int ret;
	struct device_node *np = pdev->dev.of_node;
	struct resource *res;

	printk("%s enter: dev name %s\n", __func__, dev_name(&pdev->dev));

	if (of_device_is_compatible(np, "spacemit,spacemit-snd-dma-hdmi")){
		res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
		printk("%s, start=0x%lx, end=0x%lx\n", __FUNCTION__, (unsigned long)res->start, (unsigned long)res->end);
		priv.buf_res = res;
		priv.phy_addr = res->start;
		priv.buf_base = devm_ioremap_resource(&pdev->dev, res);
		ret = snd_soc_register_component(&pdev->dev, &spacemit_snd_dma_component_hdmi, NULL, 0);
	}else{
		ret = snd_soc_register_component(&pdev->dev, &spacemit_snd_dma_component, NULL, 0);
	}

	if (ret != 0) {
		dev_err(&pdev->dev, "failed to register DAI\n");
		return ret;
	}
	return 0;
}

static int spacemit_snd_dma_pdev_remove(struct platform_device *pdev)
{
	snd_soc_unregister_component(&pdev->dev);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id spacemit_snd_dma_ids[] = {
	{ .compatible = "spacemit,spacemit-snd-dma0", },
	{ .compatible = "spacemit,spacemit-snd-dma1", },
	{ .compatible = "spacemit,spacemit-snd-dma-hdmi", },
};
#endif

static struct platform_driver spacemit_snd_dma_pdrv = {
	.driver = {
		.name = "spacemit-snd-dma",
		.of_match_table = of_match_ptr(spacemit_snd_dma_ids),
	},
	.probe = spacemit_snd_dma_pdev_probe,
	.remove = spacemit_snd_dma_pdev_remove
};

#if IS_MODULE(CONFIG_SND_SOC_SPACEMIT)
int spacemit_snd_register_dmaclient_pdrv(void)
{
	printk("%s enter\n", __FUNCTION__);
	return platform_driver_register(&spacemit_snd_dma_pdrv);
}
EXPORT_SYMBOL(spacemit_snd_register_dmaclient_pdrv);

void spacemit_snd_unregister_dmaclient_pdrv(void)
{
	platform_driver_unregister(&spacemit_snd_dma_pdrv);
}
EXPORT_SYMBOL(spacemit_snd_unregister_dmaclient_pdrv);
#else
module_platform_driver(spacemit_snd_dma_pdrv);
#endif

MODULE_DESCRIPTION("SPACEMIT Aquila ASoC PCM Platform Driver");
MODULE_LICENSE("GPL");