#define MAX_LSM_EVM_XATTR	2

const char *const lockdown_reasons[LOCKDOWN_CONFIDENTIALITY_MAX+1] = {
struct security_hook_heads security_hook_heads __lsm_ro_after_init;
static struct lsm_blob_sizes blob_sizes __lsm_ro_after_init;
static __initconst const char * const builtin_lsm_order = CONFIG_LSM;
	init_debug("%s ordering: %s (%sabled)\n", from, lsm->name,
		   is_enabled(lsm) ? "en" : "dis");
	if (*need > 0) {
		offset = *lbs;
		*lbs += *need;
		*need = offset;
	}
			init_debug("exclusive chosen: %s\n", lsm->name);
			append_ordered_lsm(lsm, "first");
				init_debug("security=%s disabled: %s\n",
			if (lsm->order == LSM_ORDER_MUTABLE &&
			    strcmp(lsm->name, name) == 0) {
				append_ordered_lsm(lsm, origin);
			init_debug("%s ignored: %s\n", origin, name);
		init_debug("%s disabled: %s\n", origin, lsm->name);
				GFP_KERNEL);
			pr_info("security= is ignored because it is superseded by lsm=\n");
	pr_info("Security Framework initializing\n");
				const char *lsm)
				  const struct cred *to, struct file *file)
int security_capget(struct task_struct *target,
		     kernel_cap_t *effective,
		     kernel_cap_t *inheritable,
		     kernel_cap_t *permitted)
				effective, inheritable, permitted);
				effective, inheritable, permitted);
                       const char *type, unsigned long flags, void *data)
int security_sb_pivotroot(const struct path *old_path, const struct path *new_path)
				void *mnt_opts,
				unsigned long kern_flags,
				unsigned long *set_kern_flags)
				mnt_opts ? -EOPNOTSUPP : 0, sb,
				mnt_opts, kern_flags, set_kern_flags);
				struct super_block *newsb,
				unsigned long kern_flags,
				unsigned long *set_kern_flags)
				kern_flags, set_kern_flags);
int security_move_mount(const struct path *from_path, const struct path *to_path)
				unsigned int obj_type)
				inode_free_by_rcu);
	hlist_for_each_entry(hp, &security_hook_heads.dentry_init_security, list) {
				name, old, new);
	struct xattr new_xattrs[MAX_LSM_EVM_XATTR + 1];
	struct xattr *lsm_xattr, *evm_xattr, *xattr;
	int ret;
	if (!initxattrs)
		return call_int_hook(inode_init_security, -EOPNOTSUPP, inode,
				     dir, qstr, NULL, NULL, NULL);
	memset(new_xattrs, 0, sizeof(new_xattrs));
	lsm_xattr = new_xattrs;
	ret = call_int_hook(inode_init_security, -EOPNOTSUPP, inode, dir, qstr,
						&lsm_xattr->name,
						&lsm_xattr->value,
						&lsm_xattr->value_len);
	if (ret)
	evm_xattr = lsm_xattr + 1;
	ret = evm_inode_init_security(inode, lsm_xattr, evm_xattr);
	for (xattr = new_xattrs; xattr->value != NULL; xattr++)
		kfree(xattr->value);
int security_old_inode_init_security(struct inode *inode, struct inode *dir,
				     const struct qstr *qstr, const char **name,
				     void **value, size_t *len)
{
	if (unlikely(IS_PRIVATE(inode)))
		return -EOPNOTSUPP;
	return call_int_hook(inode_init_security, -EOPNOTSUPP, inode, dir,
			     qstr, name, value, len);
}
EXPORT_SYMBOL(security_old_inode_init_security);

int security_path_mknod(const struct path *dir, struct dentry *dentry, umode_t mode,
			unsigned int dev)
int security_path_mkdir(const struct path *dir, struct dentry *dentry, umode_t mode)
		     (d_is_positive(new_dentry) && IS_PRIVATE(d_backing_inode(new_dentry)))))
				new_dentry, flags);
#endif
int security_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
			 struct dentry *new_dentry)
			    const char *old_name)
int security_inode_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev)
			   struct inode *new_dir, struct dentry *new_dentry,
			   unsigned int flags)
        if (unlikely(IS_PRIVATE(d_backing_inode(old_dentry)) ||
            (d_is_positive(new_dentry) && IS_PRIVATE(d_backing_inode(new_dentry)))))
						     old_dir, old_dentry);
					   new_dir, new_dentry);
int security_inode_setattr(struct user_namespace *mnt_userns,
	return evm_inode_setattr(mnt_userns, dentry, attr);
int security_inode_setxattr(struct user_namespace *mnt_userns,
	ret = call_int_hook(inode_setxattr, 1, mnt_userns, dentry, name, value,
	return evm_inode_setxattr(mnt_userns, dentry, name, value, size);
int security_inode_removexattr(struct user_namespace *mnt_userns,
	ret = call_int_hook(inode_removexattr, 1, mnt_userns, dentry, name);
		ret = cap_inode_removexattr(mnt_userns, dentry, name);
	return evm_inode_removexattr(mnt_userns, dentry, name);
int security_inode_killpriv(struct user_namespace *mnt_userns,
	return call_int_hook(inode_killpriv, 0, mnt_userns, dentry);
int security_inode_getsecurity(struct user_namespace *mnt_userns,
		rc = hp->hook.inode_getsecurity(mnt_userns, inode, name, buffer, alloc);
int security_inode_setsecurity(struct inode *inode, const char *name, const void *value, size_t size, int flags)
								flags);
int security_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size)
	 * any other error code incase of an error.
		&security_hook_heads.inode_copy_up_xattr, list) {
			unsigned long flags)
	ret = call_int_hook(mmap_file, 0, file, prot,
					mmap_prot(file, prot), flags);
	return ima_file_mmap(file, prot);
			    unsigned long prot)
				  struct fown_struct *fown, int sig)
				 int flags)
		struct rlimit *new_rlim)
			int sig, const struct cred *cred)
			 unsigned long arg4, unsigned long arg5)
			       struct msg_msg *msg, int msqflg)
			       struct task_struct *target, long type, int mode)
int security_shm_shmat(struct kern_ipc_perm *shp, char __user *shmaddr, int shmflg)
			unsigned nsops, int alter)
	return call_int_hook(inode_getsecctx, -EOPNOTSUPP, inode, ctx, ctxlen);
#endif

int security_unix_stream_connect(struct sock *sock, struct sock *other, struct sock *newsk)
						protocol, kern);
int security_socket_bind(struct socket *sock, struct sockaddr *address, int addrlen)
int security_socket_connect(struct socket *sock, struct sockaddr *address, int addrlen)
int security_socket_getpeersec_stream(struct socket *sock, char __user *optval,
				      int __user *optlen, unsigned len)
	return call_int_hook(socket_getpeersec_stream, -ENOPROTOOPT, sock,
				optval, optlen, len);
int security_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid)
	return call_int_hook(socket_getpeersec_dgram, -ENOPROTOOPT, sock,
			     skb, secid);
void security_sk_classify_flow(struct sock *sk, struct flowi_common *flic)
			struct sk_buff *skb, struct request_sock *req)
			const struct request_sock *req)
			struct sk_buff *skb)
int security_sctp_assoc_request(struct sctp_association *asoc, struct sk_buff *skb)

int security_ib_endport_manage_subnet(void *sec, const char *dev_name, u8 port_num)
	return call_int_hook(ib_endport_manage_subnet, 0, sec, dev_name, port_num);

			      struct xfrm_sec_ctx **new_ctxp)
				list) {
				0);


int security_key_getsecurity(struct key *key, char **_buffer)
	*_buffer = NULL;
	return call_int_hook(key_getsecurity, 0, key, _buffer);

