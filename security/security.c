 * Copyright (C) 2023 Microsoft Corporation <paul@paul-moore.com>
const char *const lockdown_reasons[LOCKDOWN_CONFIDENTIALITY_MAX + 1] = {
struct security_hook_heads security_hook_heads __ro_after_init;
static struct lsm_blob_sizes blob_sizes __ro_after_init;
static __initconst const char *const builtin_lsm_order = CONFIG_LSM;
	init_debug("%s ordered: %s (%s)\n", from, lsm->name,
		   is_enabled(lsm) ? "enabled" : "disabled");
	if (*need <= 0)
		return;

	offset = ALIGN(*lbs, sizeof(void *));
	*lbs = offset + *need;
	*need = offset;
	lsm_set_blob_size(&needed->lbs_xattr_count,
			  &blob_sizes.lbs_xattr_count);
			init_debug("exclusive chosen:   %s\n", lsm->name);
			append_ordered_lsm(lsm, "  first");
				init_debug("security=%s disabled: %s (only one legacy major LSM)\n",
			if (strcmp(lsm->name, name) == 0) {
				if (lsm->order == LSM_ORDER_MUTABLE)
					append_ordered_lsm(lsm, origin);
			init_debug("%s ignored: %s (not built into kernel)\n",
				   origin, name);
	/* LSM_ORDER_LAST is always last. */
	for (lsm = __start_lsm_info; lsm < __end_lsm_info; lsm++) {
		if (lsm->order == LSM_ORDER_LAST)
			append_ordered_lsm(lsm, "   last");
	}

		init_debug("%s skipped: %s (not in requested order)\n",
			   origin, lsm->name);
static void __init report_lsm_order(void)
{
	struct lsm_info **lsm, *early;
	int first = 0;

	pr_info("initializing lsm=");

	/* Report each enabled LSM name, comma separated. */
	for (early = __start_early_lsm_info;
	     early < __end_early_lsm_info; early++)
		if (is_enabled(early))
			pr_cont("%s%s", first++ == 0 ? "" : ",", early->name);
	for (lsm = ordered_lsms; *lsm; lsm++)
		if (is_enabled(*lsm))
			pr_cont("%s%s", first++ == 0 ? "" : ",", (*lsm)->name);

	pr_cont("\n");
}

			       GFP_KERNEL);
			pr_warn("security=%s is ignored because it is superseded by lsm=%s\n",
				chosen_major_lsm, chosen_lsm_order);
	report_lsm_order();

	init_debug("xattr slots          = %d\n", blob_sizes.lbs_xattr_count);
	init_debug("legacy security=%s\n", chosen_major_lsm ? : " *unspecified*");
	init_debug("  CONFIG_LSM=%s\n", builtin_lsm_order);
	init_debug("boot arg lsm=%s\n", chosen_lsm_order ? : " *unspecified*");
		init_debug("  early started: %s (%s)\n", lsm->name,
			   is_enabled(lsm) ? "enabled" : "disabled");
			       const char *lsm)
/**
 * security_binder_set_context_mgr() - Check if becoming binder ctx mgr is ok
 * @mgr: task credentials of current binder process
 *
 * Check whether @mgr is allowed to be the binder context manager.
 *
 * Return: Return 0 if permission is granted.
 */
/**
 * security_binder_transaction() - Check if a binder transaction is allowed
 * @from: sending process
 * @to: receiving process
 *
 * Check whether @from is allowed to invoke a binder transaction call to @to.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_binder_transfer_binder() - Check if a binder transfer is allowed
 * @from: sending process
 * @to: receiving process
 *
 * Check whether @from is allowed to transfer a binder reference to @to.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_binder_transfer_file() - Check if a binder file xfer is allowed
 * @from: sending process
 * @to: receiving process
 * @file: file being transferred
 *
 * Check whether @from is allowed to transfer @file to @to.
 *
 * Return: Returns 0 if permission is granted.
 */
				  const struct cred *to, const struct file *file)
/**
 * security_ptrace_access_check() - Check if tracing is allowed
 * @child: target process
 * @mode: PTRACE_MODE flags
 *
 * Check permission before allowing the current process to trace the @child
 * process.  Security modules may also want to perform a process tracing check
 * during an execve in the set_security or apply_creds hooks of tracing check
 * during an execve in the bprm_set_creds hook of binprm_security_ops if the
 * process is being traced and its security attributes would be changed by the
 * execve.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_ptrace_traceme() - Check if tracing is allowed
 * @parent: tracing process
 *
 * Check that the @parent process has sufficient permission to trace the
 * current process before allowing the current process to present itself to the
 * @parent process for tracing.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_capget() - Get the capability sets for a process
 * @target: target process
 * @effective: effective capability set
 * @inheritable: inheritable capability set
 * @permitted: permitted capability set
 *
 * Get the @effective, @inheritable, and @permitted capability sets for the
 * @target process.  The hook may also perform permission checking to determine
 * if the current process is allowed to see the capability sets of the @target
 * process.
 *
 * Return: Returns 0 if the capability sets were successfully obtained.
 */
int security_capget(const struct task_struct *target,
		    kernel_cap_t *effective,
		    kernel_cap_t *inheritable,
		    kernel_cap_t *permitted)
			     effective, inheritable, permitted);
/**
 * security_capset() - Set the capability sets for a process
 * @new: new credentials for the target process
 * @old: current credentials of the target process
 * @effective: effective capability set
 * @inheritable: inheritable capability set
 * @permitted: permitted capability set
 *
 * Set the @effective, @inheritable, and @permitted capability sets for the
 * current process.
 *
 * Return: Returns 0 and update @new if permission is granted.
 */
			     effective, inheritable, permitted);
/**
 * security_capable() - Check if a process has the necessary capability
 * @cred: credentials to examine
 * @ns: user namespace
 * @cap: capability requested
 * @opts: capability check options
 *
 * Check whether the @tsk process has the @cap capability in the indicated
 * credentials.  @cap contains the capability <include/linux/capability.h>.
 * @opts contains options for the capable check <include/linux/security.h>.
 *
 * Return: Returns 0 if the capability is granted.
 */
/**
 * security_quotactl() - Check if a quotactl() syscall is allowed for this fs
 * @cmds: commands
 * @type: type
 * @id: id
 * @sb: filesystem
 *
 * Check whether the quotactl syscall is allowed for this @sb.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_quota_on() - Check if QUOTAON is allowed for a dentry
 * @dentry: dentry
 *
 * Check whether QUOTAON is allowed for @dentry.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_syslog() - Check if accessing the kernel message ring is allowed
 * @type: SYSLOG_ACTION_* type
 *
 * Check permission before accessing the kernel message ring or changing
 * logging to the console.  See the syslog(2) manual page for an explanation of
 * the @type values.
 *
 * Return: Return 0 if permission is granted.
 */
/**
 * security_settime64() - Check if changing the system time is allowed
 * @ts: new time
 * @tz: timezone
 *
 * Check permission to change the system time, struct timespec64 is defined in
 * <include/linux/time64.h> and timezone is defined in <include/linux/time.h>.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_vm_enough_memory_mm() - Check if allocating a new mem map is allowed
 * @mm: mm struct
 * @pages: number of pages
 *
 * Check permissions for allocating a new virtual mapping.  If all LSMs return
 * a positive value, __vm_enough_memory() will be called with cap_sys_admin
 * set. If at least one LSM returns 0 or negative, __vm_enough_memory() will be
 * called with cap_sys_admin cleared.
 *
 * Return: Returns 0 if permission is granted by the LSM infrastructure to the
 *         caller.
 */
/**
 * security_bprm_creds_for_exec() - Prepare the credentials for exec()
 * @bprm: binary program information
 *
 * If the setup in prepare_exec_creds did not setup @bprm->cred->security
 * properly for executing @bprm->file, update the LSM's portion of
 * @bprm->cred->security to be what commit_creds needs to install for the new
 * program.  This hook may also optionally check permissions (e.g. for
 * transitions between security domains).  The hook must set @bprm->secureexec
 * to 1 if AT_SECURE should be set to request libc enable secure mode.  @bprm
 * contains the linux_binprm structure.
 *
 * Return: Returns 0 if the hook is successful and permission is granted.
 */
/**
 * security_bprm_creds_from_file() - Update linux_binprm creds based on file
 * @bprm: binary program information
 * @file: associated file
 *
 * If @file is setpcap, suid, sgid or otherwise marked to change privilege upon
 * exec, update @bprm->cred to reflect that change. This is called after
 * finding the binary that will be executed without an interpreter.  This
 * ensures that the credentials will not be derived from a script that the
 * binary will need to reopen, which when reopend may end up being a completely
 * different file.  This hook may also optionally check permissions (e.g. for
 * transitions between security domains).  The hook must set @bprm->secureexec
 * to 1 if AT_SECURE should be set to request libc enable secure mode.  The
 * hook must add to @bprm->per_clear any personality flags that should be
 * cleared from current->personality.  @bprm contains the linux_binprm
 * structure.
 *
 * Return: Returns 0 if the hook is successful and permission is granted.
 */
/**
 * security_bprm_check() - Mediate binary handler search
 * @bprm: binary program information
 *
 * This hook mediates the point when a search for a binary handler will begin.
 * It allows a check against the @bprm->cred->security value which was set in
 * the preceding creds_for_exec call.  The argv list and envp list are reliably
 * available in @bprm.  This hook may be called multiple times during a single
 * execve.  @bprm contains the linux_binprm structure.
 *
 * Return: Returns 0 if the hook is successful and permission is granted.
 */
/**
 * security_bprm_committing_creds() - Install creds for a process during exec()
 * @bprm: binary program information
 *
 * Prepare to install the new security attributes of a process being
 * transformed by an execve operation, based on the old credentials pointed to
 * by @current->cred and the information set in @bprm->cred by the
 * bprm_creds_for_exec hook.  @bprm points to the linux_binprm structure.  This
 * hook is a good place to perform state changes on the process such as closing
 * open file descriptors to which access will no longer be granted when the
 * attributes are changed.  This is called immediately before commit_creds().
 */
/**
 * security_bprm_committed_creds() - Tidy up after cred install during exec()
 * @bprm: binary program information
 *
 * Tidy up after the installation of the new security attributes of a process
 * being transformed by an execve operation.  The new credentials have, by this
 * point, been set to @current->cred.  @bprm points to the linux_binprm
 * structure.  This hook is a good place to perform state changes on the
 * process such as clearing out non-inheritable signal state.  This is called
 * immediately after commit_creds().
 */
/**
 * security_fs_context_submount() - Initialise fc->security
 * @fc: new filesystem context
 * @reference: dentry reference for submount/remount
 *
 * Fill out the ->security field for a new fs_context.
 *
 * Return: Returns 0 on success or negative error code on failure.
 */
int security_fs_context_submount(struct fs_context *fc, struct super_block *reference)
{
	return call_int_hook(fs_context_submount, 0, fc, reference);
}

/**
 * security_fs_context_dup() - Duplicate a fs_context LSM blob
 * @fc: destination filesystem context
 * @src_fc: source filesystem context
 *
 * Allocate and attach a security structure to sc->security.  This pointer is
 * initialised to NULL by the caller.  @fc indicates the new filesystem context.
 * @src_fc indicates the original filesystem context.
 *
 * Return: Returns 0 on success or a negative error code on failure.
 */
/**
 * security_fs_context_parse_param() - Configure a filesystem context
 * @fc: filesystem context
 * @param: filesystem parameter
 *
 * Userspace provided a parameter to configure a superblock.  The LSM can
 * consume the parameter or return it to the caller for use elsewhere.
 *
 * Return: If the parameter is used by the LSM it should return 0, if it is
 *         returned to the caller -ENOPARAM is returned, otherwise a negative
 *         error code is returned.
 */
/**
 * security_sb_alloc() - Allocate a super_block LSM blob
 * @sb: filesystem superblock
 *
 * Allocate and attach a security structure to the sb->s_security field.  The
 * s_security field is initialized to NULL when the structure is allocated.
 * @sb contains the super_block structure to be modified.
 *
 * Return: Returns 0 if operation was successful.
 */
/**
 * security_sb_delete() - Release super_block LSM associated objects
 * @sb: filesystem superblock
 *
 * Release objects tied to a superblock (e.g. inodes).  @sb contains the
 * super_block structure being released.
 */
/**
 * security_sb_free() - Free a super_block LSM blob
 * @sb: filesystem superblock
 *
 * Deallocate and clear the sb->s_security field.  @sb contains the super_block
 * structure to be modified.
 */
/**
 * security_free_mnt_opts() - Free memory associated with mount options
 * @mnt_opts: LSM processed mount options
 *
 * Free memory associated with @mnt_ops.
 */
/**
 * security_sb_eat_lsm_opts() - Consume LSM mount options
 * @options: mount options
 * @mnt_opts: LSM processed mount options
 *
 * Eat (scan @options) and save them in @mnt_opts.
 *
 * Return: Returns 0 on success, negative values on failure.
 */
/**
 * security_sb_mnt_opts_compat() - Check if new mount options are allowed
 * @sb: filesystem superblock
 * @mnt_opts: new mount options
 *
 * Determine if the new mount options in @mnt_opts are allowed given the
 * existing mounted filesystem at @sb.  @sb superblock being compared.
 *
 * Return: Returns 0 if options are compatible.
 */
/**
 * security_sb_remount() - Verify no incompatible mount changes during remount
 * @sb: filesystem superblock
 * @mnt_opts: (re)mount options
 *
 * Extracts security system specific mount options and verifies no changes are
 * being made to those options.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sb_kern_mount() - Check if a kernel mount is allowed
 * @sb: filesystem superblock
 *
 * Mount this @sb if allowed by permissions.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sb_show_options() - Output the mount options for a superblock
 * @m: output file
 * @sb: filesystem superblock
 *
 * Show (print on @m) mount options for this @sb.
 *
 * Return: Returns 0 on success, negative values on failure.
 */
/**
 * security_sb_statfs() - Check if accessing fs stats is allowed
 * @dentry: superblock handle
 *
 * Check permission before obtaining filesystem statistics for the @mnt
 * mountpoint.  @dentry is a handle on the superblock for the filesystem.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sb_mount() - Check permission for mounting a filesystem
 * @dev_name: filesystem backing device
 * @path: mount point
 * @type: filesystem type
 * @flags: mount flags
 * @data: filesystem specific data
 *
 * Check permission before an object specified by @dev_name is mounted on the
 * mount point named by @nd.  For an ordinary mount, @dev_name identifies a
 * device if the file system type requires a device.  For a remount
 * (@flags & MS_REMOUNT), @dev_name is irrelevant.  For a loopback/bind mount
 * (@flags & MS_BIND), @dev_name identifies the	pathname of the object being
 * mounted.
 *
 * Return: Returns 0 if permission is granted.
 */
		      const char *type, unsigned long flags, void *data)
/**
 * security_sb_umount() - Check permission for unmounting a filesystem
 * @mnt: mounted filesystem
 * @flags: unmount flags
 *
 * Check permission before the @mnt file system is unmounted.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sb_pivotroot() - Check permissions for pivoting the rootfs
 * @old_path: new location for current rootfs
 * @new_path: location of the new rootfs
 *
 * Check permission before pivoting the root filesystem.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_sb_pivotroot(const struct path *old_path,
			  const struct path *new_path)
/**
 * security_sb_set_mnt_opts() - Set the mount options for a filesystem
 * @sb: filesystem superblock
 * @mnt_opts: binary mount options
 * @kern_flags: kernel flags (in)
 * @set_kern_flags: kernel flags (out)
 *
 * Set the security relevant mount options used for a superblock.
 *
 * Return: Returns 0 on success, error on failure.
 */
			     void *mnt_opts,
			     unsigned long kern_flags,
			     unsigned long *set_kern_flags)
			     mnt_opts ? -EOPNOTSUPP : 0, sb,
			     mnt_opts, kern_flags, set_kern_flags);
/**
 * security_sb_clone_mnt_opts() - Duplicate superblock mount options
 * @oldsb: source superblock
 * @newsb: destination superblock
 * @kern_flags: kernel flags (in)
 * @set_kern_flags: kernel flags (out)
 *
 * Copy all security options from a given superblock to another.
 *
 * Return: Returns 0 on success, error on failure.
 */
			       struct super_block *newsb,
			       unsigned long kern_flags,
			       unsigned long *set_kern_flags)
			     kern_flags, set_kern_flags);
/**
 * security_move_mount() - Check permissions for moving a mount
 * @from_path: source mount point
 * @to_path: destination mount point
 *
 * Check permission before a mount is moved.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_move_mount(const struct path *from_path,
			const struct path *to_path)
/**
 * security_path_notify() - Check if setting a watch is allowed
 * @path: file path
 * @mask: event mask
 * @obj_type: file path type
 *
 * Check permissions before setting a watch on events as defined by @mask, on
 * an object at @path, whose type is defined by @obj_type.
 *
 * Return: Returns 0 if permission is granted.
 */
			 unsigned int obj_type)
/**
 * security_inode_alloc() - Allocate an inode LSM blob
 * @inode: the inode
 *
 * Allocate and attach a security structure to @inode->i_security.  The
 * i_security field is initialized to NULL when the inode structure is
 * allocated.
 *
 * Return: Return 0 if operation was successful.
 */
/**
 * security_inode_free() - Free an inode's LSM blob
 * @inode: the inode
 *
 * Deallocate the inode security structure and set @inode->i_security to NULL.
 */
			 inode_free_by_rcu);
/**
 * security_dentry_init_security() - Perform dentry initialization
 * @dentry: the dentry to initialize
 * @mode: mode used to determine resource type
 * @name: name of the last path component
 * @xattr_name: name of the security/LSM xattr
 * @ctx: pointer to the resulting LSM context
 * @ctxlen: length of @ctx
 *
 * Compute a context for a dentry as the inode is not yet available since NFSv4
 * has no label backed by an EA anyway.  It is important to note that
 * @xattr_name does not need to be free'd by the caller, it is a static string.
 *
 * Return: Returns 0 on success, negative values on failure.
 */
	hlist_for_each_entry(hp, &security_hook_heads.dentry_init_security,
			     list) {
/**
 * security_dentry_create_files_as() - Perform dentry initialization
 * @dentry: the dentry to initialize
 * @mode: mode used to determine resource type
 * @name: name of the last path component
 * @old: creds to use for LSM context calculations
 * @new: creds to modify
 *
 * Compute a context for a dentry as the inode is not yet available and set
 * that context in passed in creds so that new files are created using that
 * context. Context is calculated using the passed in creds and not the creds
 * of the caller.
 *
 * Return: Returns 0 on success, error on failure.
 */
			     name, old, new);
/**
 * security_inode_init_security() - Initialize an inode's LSM context
 * @inode: the inode
 * @dir: parent directory
 * @qstr: last component of the pathname
 * @initxattrs: callback function to write xattrs
 * @fs_data: filesystem specific data
 *
 * Obtain the security attribute name suffix and value to set on a newly
 * created inode and set up the incore security field for the new inode.  This
 * hook is called by the fs code as part of the inode creation transaction and
 * provides for atomic labeling of the inode, unlike the post_create/mkdir/...
 * hooks called by the VFS.
 *
 * The hook function is expected to populate the xattrs array, by calling
 * lsm_get_xattr_slot() to retrieve the slots reserved by the security module
 * with the lbs_xattr_count field of the lsm_blob_sizes structure.  For each
 * slot, the hook function should set ->name to the attribute name suffix
 * (e.g. selinux), to allocate ->value (will be freed by the caller) and set it
 * to the attribute value, to set ->value_len to the length of the value.  If
 * the security module does not use security attributes or does not wish to put
 * a security attribute on this particular inode, then it should return
 * -EOPNOTSUPP to skip this processing.
 *
 * Return: Returns 0 if the LSM successfully initialized all of the inode
 *         security attributes that are required, negative values otherwise.
 */
	struct security_hook_list *hp;
	struct xattr *new_xattrs = NULL;
	int ret = -EOPNOTSUPP, xattr_count = 0;
	if (!blob_sizes.lbs_xattr_count)
		return 0;

	if (initxattrs) {
		/* Allocate +1 for EVM and +1 as terminator. */
		new_xattrs = kcalloc(blob_sizes.lbs_xattr_count + 2,
				     sizeof(*new_xattrs), GFP_NOFS);
		if (!new_xattrs)
			return -ENOMEM;
	}

	hlist_for_each_entry(hp, &security_hook_heads.inode_init_security,
			     list) {
		ret = hp->hook.inode_init_security(inode, dir, qstr, new_xattrs,
						  &xattr_count);
		if (ret && ret != -EOPNOTSUPP)
			goto out;
		/*
		 * As documented in lsm_hooks.h, -EOPNOTSUPP in this context
		 * means that the LSM is not willing to provide an xattr, not
		 * that it wants to signal an error. Thus, continue to invoke
		 * the remaining LSMs.
		 */
	}

	/* If initxattrs() is NULL, xattr_count is zero, skip the call. */
	if (!xattr_count)
	ret = evm_inode_init_security(inode, dir, qstr, new_xattrs,
				      &xattr_count);
	for (; xattr_count > 0; xattr_count--)
		kfree(new_xattrs[xattr_count - 1].value);
	kfree(new_xattrs);
/**
 * security_inode_init_security_anon() - Initialize an anonymous inode
 * @inode: the inode
 * @name: the anonymous inode class
 * @context_inode: an optional related inode
 *
 * Set up the incore security field for the new anonymous inode and return
 * whether the inode creation is permitted by the security module or not.
 *
 * Return: Returns 0 on success, -EACCES if the security module denies the
 * creation of this inode, or another -errno upon other errors.
 */
/**
 * security_path_mknod() - Check if creating a special file is allowed
 * @dir: parent directory
 * @dentry: new file
 * @mode: new file mode
 * @dev: device number
 *
 * Check permissions when creating a file. Note that this hook is called even
 * if mknod operation is being done for a regular file.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_path_mknod(const struct path *dir, struct dentry *dentry,
			umode_t mode, unsigned int dev)
/**
 * security_path_mkdir() - Check if creating a new directory is allowed
 * @dir: parent directory
 * @dentry: new directory
 * @mode: new directory mode
 *
 * Check permissions to create a new directory in the existing directory.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_path_mkdir(const struct path *dir, struct dentry *dentry,
			umode_t mode)
/**
 * security_path_rmdir() - Check if removing a directory is allowed
 * @dir: parent directory
 * @dentry: directory to remove
 *
 * Check the permission to remove a directory.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_unlink() - Check if removing a hard link is allowed
 * @dir: parent directory
 * @dentry: file
 *
 * Check the permission to remove a hard link to a file.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_symlink() - Check if creating a symbolic link is allowed
 * @dir: parent directory
 * @dentry: symbolic link
 * @old_name: file pathname
 *
 * Check the permission to create a symbolic link to a file.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_link - Check if creating a hard link is allowed
 * @old_dentry: existing file
 * @new_dir: new parent directory
 * @new_dentry: new link
 *
 * Check permission before creating a new hard link to a file.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_rename() - Check if renaming a file is allowed
 * @old_dir: parent directory of the old file
 * @old_dentry: the old file
 * @new_dir: parent directory of the new file
 * @new_dentry: the new file
 * @flags: flags
 *
 * Check for permission to rename a file or directory.
 *
 * Return: Returns 0 if permission is granted.
 */
		     (d_is_positive(new_dentry) &&
		      IS_PRIVATE(d_backing_inode(new_dentry)))))
			     new_dentry, flags);
/**
 * security_path_truncate() - Check if truncating a file is allowed
 * @path: file
 *
 * Check permission before truncating the file indicated by path.  Note that
 * truncation permissions may also be checked based on already opened files,
 * using the security_file_truncate() hook.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_chmod() - Check if changing the file's mode is allowed
 * @path: file
 * @mode: new mode
 *
 * Check for permission to change a mode of the file @path. The new mode is
 * specified in @mode which is a bitmask of constants from
 * <include/uapi/linux/stat.h>.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_chown() - Check if changing the file's owner/group is allowed
 * @path: file
 * @uid: file owner
 * @gid: file group
 *
 * Check for permission to change owner/group of a file or directory.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_path_chroot() - Check if changing the root directory is allowed
 * @path: directory
 *
 * Check for permission to change root directory.
 *
 * Return: Returns 0 if permission is granted.
 */
#endif /* CONFIG_SECURITY_PATH */
/**
 * security_inode_create() - Check if creating a file is allowed
 * @dir: the parent directory
 * @dentry: the file being created
 * @mode: requested file mode
 *
 * Check permission to create a regular file.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_create(struct inode *dir, struct dentry *dentry,
			  umode_t mode)
/**
 * security_inode_link() - Check if creating a hard link is allowed
 * @old_dentry: existing file
 * @dir: new parent directory
 * @new_dentry: new link
 *
 * Check permission before creating a new hard link to a file.
 *
 * Return: Returns 0 if permission is granted.
 */
			struct dentry *new_dentry)
/**
 * security_inode_unlink() - Check if removing a hard link is allowed
 * @dir: parent directory
 * @dentry: file
 *
 * Check the permission to remove a hard link to a file.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_symlink() - Check if creating a symbolic link is allowed
 * @dir: parent directory
 * @dentry: symbolic link
 * @old_name: existing filename
 *
 * Check the permission to create a symbolic link to a file.
 *
 * Return: Returns 0 if permission is granted.
 */
			   const char *old_name)
/**
 * security_inode_mkdir() - Check if creation a new director is allowed
 * @dir: parent directory
 * @dentry: new directory
 * @mode: new directory mode
 *
 * Check permissions to create a new directory in the existing directory
 * associated with inode structure @dir.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_rmdir() - Check if removing a directory is allowed
 * @dir: parent directory
 * @dentry: directory to be removed
 *
 * Check the permission to remove a directory.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_mknod() - Check if creating a special file is allowed
 * @dir: parent directory
 * @dentry: new file
 * @mode: new file mode
 * @dev: device number
 *
 * Check permissions when creating a special file (or a socket or a fifo file
 * created via the mknod system call).  Note that if mknod operation is being
 * done for a regular file, then the create hook will be called and not this
 * hook.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_mknod(struct inode *dir, struct dentry *dentry,
			 umode_t mode, dev_t dev)
/**
 * security_inode_rename() - Check if renaming a file is allowed
 * @old_dir: parent directory of the old file
 * @old_dentry: the old file
 * @new_dir: parent directory of the new file
 * @new_dentry: the new file
 * @flags: flags
 *
 * Check for permission to rename a file or directory.
 *
 * Return: Returns 0 if permission is granted.
 */
			  struct inode *new_dir, struct dentry *new_dentry,
			  unsigned int flags)
	if (unlikely(IS_PRIVATE(d_backing_inode(old_dentry)) ||
		     (d_is_positive(new_dentry) &&
		      IS_PRIVATE(d_backing_inode(new_dentry)))))
					old_dir, old_dentry);
			     new_dir, new_dentry);
/**
 * security_inode_readlink() - Check if reading a symbolic link is allowed
 * @dentry: link
 *
 * Check the permission to read the symbolic link.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_follow_link() - Check if following a symbolic link is allowed
 * @dentry: link dentry
 * @inode: link inode
 * @rcu: true if in RCU-walk mode
 *
 * Check permission to follow a symbolic link when looking up a pathname.  If
 * @rcu is true, @inode is not stable.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_permission() - Check if accessing an inode is allowed
 * @inode: inode
 * @mask: access mask
 *
 * Check permission before accessing an inode.  This hook is called by the
 * existing Linux permission function, so a security module can use it to
 * provide additional checking for existing Linux permission checks.  Notice
 * that this hook is called when a file is opened (as well as many other
 * operations), whereas the file_security_ops permission hook is called when
 * the actual read/write operations are performed.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_setattr() - Check if setting file attributes is allowed
 * @idmap: idmap of the mount
 * @dentry: file
 * @attr: new attributes
 *
 * Check permission before setting file attributes.  Note that the kernel call
 * to notify_change is performed from several locations, whenever file
 * attributes change (such as when a file is truncated, chown/chmod operations,
 * transferring disk quotas, etc).
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_setattr(struct mnt_idmap *idmap,
	return evm_inode_setattr(idmap, dentry, attr);
/**
 * security_inode_getattr() - Check if getting file attributes is allowed
 * @path: file
 *
 * Check permission before obtaining file attributes.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_setxattr() - Check if setting file xattrs is allowed
 * @idmap: idmap of the mount
 * @dentry: file
 * @name: xattr name
 * @value: xattr value
 * @size: size of xattr value
 * @flags: flags
 *
 * Check permission before setting the extended attributes.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_setxattr(struct mnt_idmap *idmap,
	ret = call_int_hook(inode_setxattr, 1, idmap, dentry, name, value,
	return evm_inode_setxattr(idmap, dentry, name, value, size);
}

/**
 * security_inode_set_acl() - Check if setting posix acls is allowed
 * @idmap: idmap of the mount
 * @dentry: file
 * @acl_name: acl name
 * @kacl: acl struct
 *
 * Check permission before setting posix acls, the posix acls in @kacl are
 * identified by @acl_name.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_set_acl(struct mnt_idmap *idmap,
			   struct dentry *dentry, const char *acl_name,
			   struct posix_acl *kacl)
{
	int ret;

	if (unlikely(IS_PRIVATE(d_backing_inode(dentry))))
		return 0;
	ret = call_int_hook(inode_set_acl, 0, idmap, dentry, acl_name,
			    kacl);
	if (ret)
		return ret;
	ret = ima_inode_set_acl(idmap, dentry, acl_name, kacl);
	if (ret)
		return ret;
	return evm_inode_set_acl(idmap, dentry, acl_name, kacl);
}

/**
 * security_inode_get_acl() - Check if reading posix acls is allowed
 * @idmap: idmap of the mount
 * @dentry: file
 * @acl_name: acl name
 *
 * Check permission before getting osix acls, the posix acls are identified by
 * @acl_name.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_get_acl(struct mnt_idmap *idmap,
			   struct dentry *dentry, const char *acl_name)
{
	if (unlikely(IS_PRIVATE(d_backing_inode(dentry))))
		return 0;
	return call_int_hook(inode_get_acl, 0, idmap, dentry, acl_name);
}

/**
 * security_inode_remove_acl() - Check if removing a posix acl is allowed
 * @idmap: idmap of the mount
 * @dentry: file
 * @acl_name: acl name
 *
 * Check permission before removing posix acls, the posix acls are identified
 * by @acl_name.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_remove_acl(struct mnt_idmap *idmap,
			      struct dentry *dentry, const char *acl_name)
{
	int ret;

	if (unlikely(IS_PRIVATE(d_backing_inode(dentry))))
		return 0;
	ret = call_int_hook(inode_remove_acl, 0, idmap, dentry, acl_name);
	if (ret)
		return ret;
	ret = ima_inode_remove_acl(idmap, dentry, acl_name);
	if (ret)
		return ret;
	return evm_inode_remove_acl(idmap, dentry, acl_name);
/**
 * security_inode_post_setxattr() - Update the inode after a setxattr operation
 * @dentry: file
 * @name: xattr name
 * @value: xattr value
 * @size: xattr value size
 * @flags: flags
 *
 * Update inode security field after successful setxattr operation.
 */
/**
 * security_inode_getxattr() - Check if xattr access is allowed
 * @dentry: file
 * @name: xattr name
 *
 * Check permission before obtaining the extended attributes identified by
 * @name for @dentry.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_listxattr() - Check if listing xattrs is allowed
 * @dentry: file
 *
 * Check permission before obtaining the list of extended attribute names for
 * @dentry.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_inode_removexattr() - Check if removing an xattr is allowed
 * @idmap: idmap of the mount
 * @dentry: file
 * @name: xattr name
 *
 * Check permission before removing the extended attribute identified by @name
 * for @dentry.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_inode_removexattr(struct mnt_idmap *idmap,
	ret = call_int_hook(inode_removexattr, 1, idmap, dentry, name);
		ret = cap_inode_removexattr(idmap, dentry, name);
	return evm_inode_removexattr(idmap, dentry, name);
/**
 * security_inode_need_killpriv() - Check if security_inode_killpriv() required
 * @dentry: associated dentry
 *
 * Called when an inode has been changed to determine if
 * security_inode_killpriv() should be called.
 *
 * Return: Return <0 on error to abort the inode change operation, return 0 if
 *         security_inode_killpriv() does not need to be called, return >0 if
 *         security_inode_killpriv() does need to be called.
 */
/**
 * security_inode_killpriv() - The setuid bit is removed, update LSM state
 * @idmap: idmap of the mount
 * @dentry: associated dentry
 *
 * The @dentry's setuid bit is being removed.  Remove similar security labels.
 * Called with the dentry->d_inode->i_mutex held.
 *
 * Return: Return 0 on success.  If error is returned, then the operation
 *         causing setuid bit removal is failed.
 */
int security_inode_killpriv(struct mnt_idmap *idmap,
	return call_int_hook(inode_killpriv, 0, idmap, dentry);
/**
 * security_inode_getsecurity() - Get the xattr security label of an inode
 * @idmap: idmap of the mount
 * @inode: inode
 * @name: xattr name
 * @buffer: security label buffer
 * @alloc: allocation flag
 *
 * Retrieve a copy of the extended attribute representation of the security
 * label associated with @name for @inode via @buffer.  Note that @name is the
 * remainder of the attribute name after the security prefix has been removed.
 * @alloc is used to specify if the call should return a value via the buffer
 * or just the value length.
 *
 * Return: Returns size of buffer on success.
 */
int security_inode_getsecurity(struct mnt_idmap *idmap,
		rc = hp->hook.inode_getsecurity(idmap, inode, name, buffer,
						alloc);
/**
 * security_inode_setsecurity() - Set the xattr security label of an inode
 * @inode: inode
 * @name: xattr name
 * @value: security label
 * @size: length of security label
 * @flags: flags
 *
 * Set the security label associated with @name for @inode from the extended
 * attribute value @value.  @size indicates the size of the @value in bytes.
 * @flags may be XATTR_CREATE, XATTR_REPLACE, or 0. Note that @name is the
 * remainder of the attribute name after the security. prefix has been removed.
 *
 * Return: Returns 0 on success.
 */
int security_inode_setsecurity(struct inode *inode, const char *name,
			       const void *value, size_t size, int flags)
						flags);
/**
 * security_inode_listsecurity() - List the xattr security label names
 * @inode: inode
 * @buffer: buffer
 * @buffer_size: size of buffer
 *
 * Copy the extended attribute names for the security labels associated with
 * @inode into @buffer.  The maximum size of @buffer is specified by
 * @buffer_size.  @buffer may be NULL to request the size of the buffer
 * required.
 *
 * Return: Returns number of bytes used/required on success.
 */
int security_inode_listsecurity(struct inode *inode,
				char *buffer, size_t buffer_size)
/**
 * security_inode_getsecid() - Get an inode's secid
 * @inode: inode
 * @secid: secid to return
 *
 * Get the secid associated with the node.  In case of failure, @secid will be
 * set to zero.
 */
/**
 * security_inode_copy_up() - Create new creds for an overlayfs copy-up op
 * @src: union dentry of copy-up file
 * @new: newly created creds
 *
 * A file is about to be copied up from lower layer to upper layer of overlay
 * filesystem. Security module can prepare a set of new creds and modify as
 * need be and return new creds. Caller will switch to new creds temporarily to
 * create new file and release newly allocated creds.
 *
 * Return: Returns 0 on success or a negative error code on error.
 */
/**
 * security_inode_copy_up_xattr() - Filter xattrs in an overlayfs copy-up op
 * @name: xattr name
 *
 * Filter the xattrs being copied up when a unioned file is copied up from a
 * lower layer to the union/overlay layer.   The caller is responsible for
 * reading and writing the xattrs, this hook is merely a filter.
 *
 * Return: Returns 0 to accept the xattr, 1 to discard the xattr, -EOPNOTSUPP
 *         if the security module does not know about attribute, or a negative
 *         error code to abort the copy up.
 */
	 * any other error code in case of an error.
			     &security_hook_heads.inode_copy_up_xattr, list) {
/**
 * security_kernfs_init_security() - Init LSM context for a kernfs node
 * @kn_dir: parent kernfs node
 * @kn: the kernfs node to initialize
 *
 * Initialize the security context of a newly created kernfs node based on its
 * own and its parent's attributes.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_permission() - Check file permissions
 * @file: file
 * @mask: requested permissions
 *
 * Check file permissions before accessing an open file.  This hook is called
 * by various operations that read or write files.  A security module can use
 * this hook to perform additional checking on these operations, e.g. to
 * revalidate permissions on use to support privilege bracketing or policy
 * changes.  Notice that this hook is used when the actual read/write
 * operations are performed, whereas the inode_security_ops hook is called when
 * a file is opened (as well as many other operations).  Although this hook can
 * be used to revalidate permissions for various system call operations that
 * read or write files, it does not address the revalidation of permissions for
 * memory-mapped files.  Security modules must handle this separately if they
 * need such revalidation.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_alloc() - Allocate and init a file's LSM blob
 * @file: the file
 *
 * Allocate and attach a security structure to the file->f_security field.  The
 * security field is initialized to NULL when the structure is first created.
 *
 * Return: Return 0 if the hook is successful and permission is granted.
 */
/**
 * security_file_free() - Free a file's LSM blob
 * @file: the file
 *
 * Deallocate and free any security structures stored in file->f_security.
 */
/**
 * security_file_ioctl() - Check if an ioctl is allowed
 * @file: associated file
 * @cmd: ioctl cmd
 * @arg: ioctl arguments
 *
 * Check permission for an ioctl operation on @file.  Note that @arg sometimes
 * represents a user space pointer; in other cases, it may be a simple integer
 * value.  When @arg represents a user space pointer, it should never be used
 * by the security module.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_ioctl_compat() - Check if an ioctl is allowed in compat mode
 * @file: associated file
 * @cmd: ioctl cmd
 * @arg: ioctl arguments
 *
 * Compat version of security_file_ioctl() that correctly handles 32-bit
 * processes running on 64-bit kernels.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_file_ioctl_compat(struct file *file, unsigned int cmd,
			       unsigned long arg)
{
	return call_int_hook(file_ioctl_compat, 0, file, cmd, arg);
}
EXPORT_SYMBOL_GPL(security_file_ioctl_compat);

/**
 * security_mmap_file() - Check if mmap'ing a file is allowed
 * @file: file
 * @prot: protection applied by the kernel
 * @flags: flags
 *
 * Check permissions for a mmap operation.  The @file may be NULL, e.g. if
 * mapping anonymous memory.
 *
 * Return: Returns 0 if permission is granted.
 */
		       unsigned long flags)
	unsigned long prot_adj = mmap_prot(file, prot);

	ret = call_int_hook(mmap_file, 0, file, prot, prot_adj, flags);
	return ima_file_mmap(file, prot, prot_adj, flags);
/**
 * security_mmap_addr() - Check if mmap'ing an address is allowed
 * @addr: address
 *
 * Check permissions for a mmap operation at @addr.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_mprotect() - Check if changing memory protections is allowed
 * @vma: memory region
 * @reqprot: application requested protection
 * @prot: protection applied by the kernel
 *
 * Check permissions before changing memory access permissions.
 *
 * Return: Returns 0 if permission is granted.
 */
			   unsigned long prot)
/**
 * security_file_lock() - Check if a file lock is allowed
 * @file: file
 * @cmd: lock operation (e.g. F_RDLCK, F_WRLCK)
 *
 * Check permission before performing file locking operations.  Note the hook
 * mediates both flock and fcntl style locks.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_fcntl() - Check if fcntl() op is allowed
 * @file: file
 * @cmd: fcntl command
 * @arg: command argument
 *
 * Check permission before allowing the file operation specified by @cmd from
 * being performed on the file @file.  Note that @arg sometimes represents a
 * user space pointer; in other cases, it may be a simple integer value.  When
 * @arg represents a user space pointer, it should never be used by the
 * security module.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_set_fowner() - Set the file owner info in the LSM blob
 * @file: the file
 *
 * Save owner security information (typically from current->security) in
 * file->f_security for later use by the send_sigiotask hook.
 *
 * Return: Returns 0 on success.
 */
/**
 * security_file_send_sigiotask() - Check if sending SIGIO/SIGURG is allowed
 * @tsk: target task
 * @fown: signal sender
 * @sig: signal to be sent, SIGIO is sent if 0
 *
 * Check permission for the file owner @fown to send SIGIO or SIGURG to the
 * process @tsk.  Note that this hook is sometimes called from interrupt.  Note
 * that the fown_struct, @fown, is never outside the context of a struct file,
 * so the file structure (and associated security information) can always be
 * obtained: container_of(fown, struct file, f_owner).
 *
 * Return: Returns 0 if permission is granted.
 */
				 struct fown_struct *fown, int sig)
/**
 * security_file_receive() - Check is receiving a file via IPC is allowed
 * @file: file being received
 *
 * This hook allows security modules to control the ability of a process to
 * receive an open file descriptor via socket IPC.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_open() - Save open() time state for late use by the LSM
 * @file:
 *
 * Save open-time permission checking state for later use upon file_permission,
 * and recheck access if anything has changed since inode_permission.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_file_truncate() - Check if truncating a file is allowed
 * @file: file
 *
 * Check permission before truncating a file, i.e. using ftruncate.  Note that
 * truncation permission may also be checked based on the path, using the
 * @path_truncate hook.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_file_truncate(struct file *file)
{
	return call_int_hook(file_truncate, 0, file);
}

/**
 * security_task_alloc() - Allocate a task's LSM blob
 * @task: the task
 * @clone_flags: flags indicating what is being shared
 *
 * Handle allocation of task-related resources.
 *
 * Return: Returns a zero on success, negative values on failure.
 */
/**
 * security_task_free() - Free a task's LSM blob and related resources
 * @task: task
 *
 * Handle release of task-related resources.  Note that this can be called from
 * interrupt context.
 */
/**
 * security_cred_alloc_blank() - Allocate the min memory to allow cred_transfer
 * @cred: credentials
 * @gfp: gfp flags
 *
 * Only allocate sufficient memory and attach to @cred such that
 * cred_transfer() will not get ENOMEM.
 *
 * Return: Returns 0 on success, negative values on failure.
 */
/**
 * security_cred_free() - Free the cred's LSM blob and associated resources
 * @cred: credentials
 *
 * Deallocate and clear the cred->security field in a set of credentials.
 */
/**
 * security_prepare_creds() - Prepare a new set of credentials
 * @new: new credentials
 * @old: original credentials
 * @gfp: gfp flags
 *
 * Prepare a new set of credentials by copying the data from the old set.
 *
 * Return: Returns 0 on success, negative values on failure.
 */
/**
 * security_transfer_creds() - Transfer creds
 * @new: target credentials
 * @old: original credentials
 *
 * Transfer data from original creds to new creds.
 */
/**
 * security_cred_getsecid() - Get the secid from a set of credentials
 * @c: credentials
 * @secid: secid value
 *
 * Retrieve the security identifier of the cred structure @c.  In case of
 * failure, @secid will be set to zero.
 */
/**
 * security_kernel_act_as() - Set the kernel credentials to act as secid
 * @new: credentials
 * @secid: secid
 *
 * Set the credentials for a kernel service to act as (subjective context).
 * The current task must be the one that nominated @secid.
 *
 * Return: Returns 0 if successful.
 */
/**
 * security_kernel_create_files_as() - Set file creation context using an inode
 * @new: target credentials
 * @inode: reference inode
 *
 * Set the file creation context in a set of credentials to be the same as the
 * objective context of the specified inode.  The current task must be the one
 * that nominated @inode.
 *
 * Return: Returns 0 if successful.
 */
/**
 * security_kernel_module_request() - Check is loading a module is allowed
 * @kmod_name: module name
 *
 * Ability to trigger the kernel to automatically upcall to userspace for
 * userspace to load a kernel module with the given name.
 *
 * Return: Returns 0 if successful.
 */
/**
 * security_kernel_read_file() - Read a file specified by userspace
 * @file: file
 * @id: file identifier
 * @contents: trust if security_kernel_post_read_file() will be called
 *
 * Read a file specified by userspace.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_kernel_post_read_file() - Read a file specified by userspace
 * @file: file
 * @buf: file contents
 * @size: size of file contents
 * @id: file identifier
 *
 * Read a file specified by userspace.  This must be paired with a prior call
 * to security_kernel_read_file() call that indicated this hook would also be
 * called, see security_kernel_read_file() for more information.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_kernel_post_read_file(struct file *file, char *buf, loff_t size,
				   enum kernel_read_file_id id)
/**
 * security_kernel_load_data() - Load data provided by userspace
 * @id: data identifier
 * @contents: true if security_kernel_post_load_data() will be called
 *
 * Load data provided by userspace.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_kernel_post_load_data() - Load userspace data from a non-file source
 * @buf: data
 * @size: size of data
 * @id: data identifier
 * @description: text description of data, specific to the id value
 *
 * Load data provided by a non-file source (usually userspace buffer).  This
 * must be paired with a prior security_kernel_load_data() call that indicated
 * this hook would also be called, see security_kernel_load_data() for more
 * information.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_fix_setuid() - Update LSM with new user id attributes
 * @new: updated credentials
 * @old: credentials being replaced
 * @flags: LSM_SETID_* flag values
 *
 * Update the module's state after setting one or more of the user identity
 * attributes of the current process.  The @flags parameter indicates which of
 * the set*uid system calls invoked this hook.  If @new is the set of
 * credentials that will be installed.  Modifications should be made to this
 * rather than to @current->cred.
 *
 * Return: Returns 0 on success.
 */
/**
 * security_task_fix_setgid() - Update LSM with new group id attributes
 * @new: updated credentials
 * @old: credentials being replaced
 * @flags: LSM_SETID_* flag value
 *
 * Update the module's state after setting one or more of the group identity
 * attributes of the current process.  The @flags parameter indicates which of
 * the set*gid system calls invoked this hook.  @new is the set of credentials
 * that will be installed.  Modifications should be made to this rather than to
 * @current->cred.
 *
 * Return: Returns 0 on success.
 */
			     int flags)
/**
 * security_task_fix_setgroups() - Update LSM with new supplementary groups
 * @new: updated credentials
 * @old: credentials being replaced
 *
 * Update the module's state after setting the supplementary group identity
 * attributes of the current process.  @new is the set of credentials that will
 * be installed.  Modifications should be made to this rather than to
 * @current->cred.
 *
 * Return: Returns 0 on success.
 */
/**
 * security_task_setpgid() - Check if setting the pgid is allowed
 * @p: task being modified
 * @pgid: new pgid
 *
 * Check permission before setting the process group identifier of the process
 * @p to @pgid.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_getpgid() - Check if getting the pgid is allowed
 * @p: task
 *
 * Check permission before getting the process group identifier of the process
 * @p.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_getsid() - Check if getting the session id is allowed
 * @p: task
 *
 * Check permission before getting the session identifier of the process @p.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_current_getsecid_subj() - Get the current task's subjective secid
 * @secid: secid value
 *
 * Retrieve the subjective security identifier of the current task and return
 * it in @secid.  In case of failure, @secid will be set to zero.
 */
/**
 * security_task_getsecid_obj() - Get a task's objective secid
 * @p: target task
 * @secid: secid value
 *
 * Retrieve the objective security identifier of the task_struct in @p and
 * return it in @secid. In case of failure, @secid will be set to zero.
 */
/**
 * security_task_setnice() - Check if setting a task's nice value is allowed
 * @p: target task
 * @nice: nice value
 *
 * Check permission before setting the nice value of @p to @nice.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_setioprio() - Check if setting a task's ioprio is allowed
 * @p: target task
 * @ioprio: ioprio value
 *
 * Check permission before setting the ioprio value of @p to @ioprio.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_getioprio() - Check if getting a task's ioprio is allowed
 * @p: task
 *
 * Check permission before getting the ioprio value of @p.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_prlimit() - Check if get/setting resources limits is allowed
 * @cred: current task credentials
 * @tcred: target task credentials
 * @flags: LSM_PRLIMIT_* flag bits indicating a get/set/both
 *
 * Check permission before getting and/or setting the resource limits of
 * another task.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_setrlimit() - Check if setting a new rlimit value is allowed
 * @p: target task's group leader
 * @resource: resource whose limit is being set
 * @new_rlim: new resource limit
 *
 * Check permission before setting the resource limits of process @p for
 * @resource to @new_rlim.  The old resource limit values can be examined by
 * dereferencing (p->signal->rlim + resource).
 *
 * Return: Returns 0 if permission is granted.
 */
			    struct rlimit *new_rlim)
/**
 * security_task_setscheduler() - Check if setting sched policy/param is allowed
 * @p: target task
 *
 * Check permission before setting scheduling policy and/or parameters of
 * process @p.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_getscheduler() - Check if getting scheduling info is allowed
 * @p: target task
 *
 * Check permission before obtaining scheduling information for process @p.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_movememory() - Check if moving memory is allowed
 * @p: task
 *
 * Check permission before moving memory owned by process @p.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_task_kill() - Check if sending a signal is allowed
 * @p: target process
 * @info: signal information
 * @sig: signal value
 * @cred: credentials of the signal sender, NULL if @current
 *
 * Check permission before sending signal @sig to @p.  @info can be NULL, the
 * constant 1, or a pointer to a kernel_siginfo structure.  If @info is 1 or
 * SI_FROMKERNEL(info) is true, then the signal should be viewed as coming from
 * the kernel and should typically be permitted.  SIGIO signals are handled
 * separately by the send_sigiotask hook in file_security_ops.
 *
 * Return: Returns 0 if permission is granted.
 */
		       int sig, const struct cred *cred)
/**
 * security_task_prctl() - Check if a prctl op is allowed
 * @option: operation
 * @arg2: argument
 * @arg3: argument
 * @arg4: argument
 * @arg5: argument
 *
 * Check permission before performing a process control operation on the
 * current process.
 *
 * Return: Return -ENOSYS if no-one wanted to handle this op, any other value
 *         to cause prctl() to return immediately with that value.
 */
			unsigned long arg4, unsigned long arg5)
/**
 * security_task_to_inode() - Set the security attributes of a task's inode
 * @p: task
 * @inode: inode
 *
 * Set the security attributes for an inode based on an associated task's
 * security attributes, e.g. for /proc/pid inodes.
 */
/**
 * security_create_user_ns() - Check if creating a new userns is allowed
 * @cred: prepared creds
 *
 * Check permission prior to creating a new user namespace.
 *
 * Return: Returns 0 if successful, otherwise < 0 error code.
 */
/**
 * security_ipc_permission() - Check if sysv ipc access is allowed
 * @ipcp: ipc permission structure
 * @flag: requested permissions
 *
 * Check permissions for access to IPC.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_ipc_getsecid() - Get the sysv ipc object's secid
 * @ipcp: ipc permission structure
 * @secid: secid pointer
 *
 * Get the secid associated with the ipc object.  In case of failure, @secid
 * will be set to zero.
 */
/**
 * security_msg_msg_alloc() - Allocate a sysv ipc message LSM blob
 * @msg: message structure
 *
 * Allocate and attach a security structure to the msg->security field.  The
 * security field is initialized to NULL when the structure is first created.
 *
 * Return: Return 0 if operation was successful and permission is granted.
 */
/**
 * security_msg_msg_free() - Free a sysv ipc message LSM blob
 * @msg: message structure
 *
 * Deallocate the security structure for this message.
 */
/**
 * security_msg_queue_alloc() - Allocate a sysv ipc msg queue LSM blob
 * @msq: sysv ipc permission structure
 *
 * Allocate and attach a security structure to @msg. The security field is
 * initialized to NULL when the structure is first created.
 *
 * Return: Returns 0 if operation was successful and permission is granted.
 */
/**
 * security_msg_queue_free() - Free a sysv ipc msg queue LSM blob
 * @msq: sysv ipc permission structure
 *
 * Deallocate security field @perm->security for the message queue.
 */
/**
 * security_msg_queue_associate() - Check if a msg queue operation is allowed
 * @msq: sysv ipc permission structure
 * @msqflg: operation flags
 *
 * Check permission when a message queue is requested through the msgget system
 * call. This hook is only called when returning the message queue identifier
 * for an existing message queue, not when a new message queue is created.
 *
 * Return: Return 0 if permission is granted.
 */
/**
 * security_msg_queue_msgctl() - Check if a msg queue operation is allowed
 * @msq: sysv ipc permission structure
 * @cmd: operation
 *
 * Check permission when a message control operation specified by @cmd is to be
 * performed on the message queue with permissions.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_msg_queue_msgsnd() - Check if sending a sysv ipc message is allowed
 * @msq: sysv ipc permission structure
 * @msg: message
 * @msqflg: operation flags
 *
 * Check permission before a message, @msg, is enqueued on the message queue
 * with permissions specified in @msq.
 *
 * Return: Returns 0 if permission is granted.
 */
			      struct msg_msg *msg, int msqflg)
/**
 * security_msg_queue_msgrcv() - Check if receiving a sysv ipc msg is allowed
 * @msq: sysv ipc permission structure
 * @msg: message
 * @target: target task
 * @type: type of message requested
 * @mode: operation flags
 *
 * Check permission before a message, @msg, is removed from the message	queue.
 * The @target task structure contains a pointer to the process that will be
 * receiving the message (not equal to the current process when inline receives
 * are being performed).
 *
 * Return: Returns 0 if permission is granted.
 */
			      struct task_struct *target, long type, int mode)
/**
 * security_shm_alloc() - Allocate a sysv shm LSM blob
 * @shp: sysv ipc permission structure
 *
 * Allocate and attach a security structure to the @shp security field.  The
 * security field is initialized to NULL when the structure is first created.
 *
 * Return: Returns 0 if operation was successful and permission is granted.
 */
/**
 * security_shm_free() - Free a sysv shm LSM blob
 * @shp: sysv ipc permission structure
 *
 * Deallocate the security structure @perm->security for the memory segment.
 */
/**
 * security_shm_associate() - Check if a sysv shm operation is allowed
 * @shp: sysv ipc permission structure
 * @shmflg: operation flags
 *
 * Check permission when a shared memory region is requested through the shmget
 * system call. This hook is only called when returning the shared memory
 * region identifier for an existing region, not when a new shared memory
 * region is created.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_shm_shmctl() - Check if a sysv shm operation is allowed
 * @shp: sysv ipc permission structure
 * @cmd: operation
 *
 * Check permission when a shared memory control operation specified by @cmd is
 * to be performed on the shared memory region with permissions in @shp.
 *
 * Return: Return 0 if permission is granted.
 */
/**
 * security_shm_shmat() - Check if a sysv shm attach operation is allowed
 * @shp: sysv ipc permission structure
 * @shmaddr: address of memory region to attach
 * @shmflg: operation flags
 *
 * Check permissions prior to allowing the shmat system call to attach the
 * shared memory segment with permissions @shp to the data segment of the
 * calling process. The attaching address is specified by @shmaddr.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_shm_shmat(struct kern_ipc_perm *shp,
		       char __user *shmaddr, int shmflg)
/**
 * security_sem_alloc() - Allocate a sysv semaphore LSM blob
 * @sma: sysv ipc permission structure
 *
 * Allocate and attach a security structure to the @sma security field. The
 * security field is initialized to NULL when the structure is first created.
 *
 * Return: Returns 0 if operation was successful and permission is granted.
 */
/**
 * security_sem_free() - Free a sysv semaphore LSM blob
 * @sma: sysv ipc permission structure
 *
 * Deallocate security structure @sma->security for the semaphore.
 */
/**
 * security_sem_associate() - Check if a sysv semaphore operation is allowed
 * @sma: sysv ipc permission structure
 * @semflg: operation flags
 *
 * Check permission when a semaphore is requested through the semget system
 * call. This hook is only called when returning the semaphore identifier for
 * an existing semaphore, not when a new one must be created.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sem_semctl() - Check if a sysv semaphore operation is allowed
 * @sma: sysv ipc permission structure
 * @cmd: operation
 *
 * Check permission when a semaphore operation specified by @cmd is to be
 * performed on the semaphore.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sem_semop() - Check if a sysv semaphore operation is allowed
 * @sma: sysv ipc permission structure
 * @sops: operations to perform
 * @nsops: number of operations
 * @alter: flag indicating changes will be made
 *
 * Check permissions before performing operations on members of the semaphore
 * set. If the @alter flag is nonzero, the semaphore set may be modified.
 *
 * Return: Returns 0 if permission is granted.
 */
		       unsigned nsops, int alter)
/**
 * security_d_instantiate() - Populate an inode's LSM state based on a dentry
 * @dentry: dentry
 * @inode: inode
 *
 * Fill in @inode security information for a @dentry if allowed.
 */
/**
 * security_getprocattr() - Read an attribute for a task
 * @p: the task
 * @lsm: LSM name
 * @name: attribute name
 * @value: attribute value
 *
 * Read attribute @name for task @p and store it into @value if allowed.
 *
 * Return: Returns the length of @value on success, a negative value otherwise.
 */
/**
 * security_setprocattr() - Set an attribute for a task
 * @lsm: LSM name
 * @name: attribute name
 * @value: attribute value
 * @size: attribute value size
 *
 * Write (set) the current task's attribute @name to @value, size @size if
 * allowed.
 *
 * Return: Returns bytes written on success, a negative value otherwise.
 */
/**
 * security_netlink_send() - Save info and check if netlink sending is allowed
 * @sk: sending socket
 * @skb: netlink message
 *
 * Save security information for a netlink message so that permission checking
 * can be performed when the message is processed.  The security information
 * can be saved using the eff_cap field of the netlink_skb_parms structure.
 * Also may be used to provide fine grained control over message transmission.
 *
 * Return: Returns 0 if the information was successfully saved and message is
 *         allowed to be transmitted.
 */
/**
 * security_ismaclabel() - Check is the named attribute is a MAC label
 * @name: full extended attribute name
 *
 * Check if the extended attribute specified by @name represents a MAC label.
 *
 * Return: Returns 1 if name is a MAC attribute otherwise returns 0.
 */
/**
 * security_secid_to_secctx() - Convert a secid to a secctx
 * @secid: secid
 * @secdata: secctx
 * @seclen: secctx length
 *
 * Convert secid to security context.  If @secdata is NULL the length of the
 * result will be returned in @seclen, but no @secdata will be returned.  This
 * does mean that the length could change between calls to check the length and
 * the next call which actually allocates and returns the @secdata.
 *
 * Return: Return 0 on success, error on failure.
 */
/**
 * security_secctx_to_secid() - Convert a secctx to a secid
 * @secdata: secctx
 * @seclen: length of secctx
 * @secid: secid
 *
 * Convert security context to secid.
 *
 * Return: Returns 0 on success, error on failure.
 */
/**
 * security_release_secctx() - Free a secctx buffer
 * @secdata: secctx
 * @seclen: length of secctx
 *
 * Release the security context.
 */
/**
 * security_inode_invalidate_secctx() - Invalidate an inode's security label
 * @inode: inode
 *
 * Notify the security module that it must revalidate the security context of
 * an inode.
 */
/**
 * security_inode_notifysecctx() - Nofify the LSM of an inode's security label
 * @inode: inode
 * @ctx: secctx
 * @ctxlen: length of secctx
 *
 * Notify the security module of what the security context of an inode should
 * be.  Initializes the incore security context managed by the security module
 * for this inode.  Example usage: NFS client invokes this hook to initialize
 * the security context in its incore inode to the value provided by the server
 * for the file when the server returned the file's attributes to the client.
 * Must be called with inode->i_mutex locked.
 *
 * Return: Returns 0 on success, error on failure.
 */
/**
 * security_inode_setsecctx() - Change the security label of an inode
 * @dentry: inode
 * @ctx: secctx
 * @ctxlen: length of secctx
 *
 * Change the security context of an inode.  Updates the incore security
 * context managed by the security module and invokes the fs code as needed
 * (via __vfs_setxattr_noperm) to update any backing xattrs that represent the
 * context.  Example usage: NFS server invokes this hook to change the security
 * context in its incore inode and on the backing filesystem to a value
 * provided by the client on a SETATTR operation.  Must be called with
 * inode->i_mutex locked.
 *
 * Return: Returns 0 on success, error on failure.
 */
/**
 * security_inode_getsecctx() - Get the security label of an inode
 * @inode: inode
 * @ctx: secctx
 * @ctxlen: length of secctx
 *
 * On success, returns 0 and fills out @ctx and @ctxlen with the security
 * context for the given @inode.
 *
 * Return: Returns 0 on success, error on failure.
 */
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.inode_getsecctx, list) {
		rc = hp->hook.inode_getsecctx(inode, ctx, ctxlen);
		if (rc != LSM_RET_DEFAULT(inode_getsecctx))
			return rc;
	}

	return LSM_RET_DEFAULT(inode_getsecctx);
/**
 * security_post_notification() - Check if a watch notification can be posted
 * @w_cred: credentials of the task that set the watch
 * @cred: credentials of the task which triggered the watch
 * @n: the notification
 *
 * Check to see if a watch notification can be posted to a particular queue.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_watch_key() - Check if a task is allowed to watch for key events
 * @key: the key to watch
 *
 * Check to see if a process is allowed to watch for event notifications from
 * a key or keyring.
 *
 * Return: Returns 0 if permission is granted.
 */
#endif /* CONFIG_KEY_NOTIFICATIONS */
/**
 * security_unix_stream_connect() - Check if a AF_UNIX stream is allowed
 * @sock: originating sock
 * @other: peer sock
 * @newsk: new sock
 *
 * Check permissions before establishing a Unix domain stream connection
 * between @sock and @other.
 *
 * The @unix_stream_connect and @unix_may_send hooks were necessary because
 * Linux provides an alternative to the conventional file name space for Unix
 * domain sockets.  Whereas binding and connecting to sockets in the file name
 * space is mediated by the typical file permissions (and caught by the mknod
 * and permission hooks in inode_security_ops), binding and connecting to
 * sockets in the abstract name space is completely unmediated.  Sufficient
 * control of Unix domain sockets in the abstract name space isn't possible
 * using only the socket layer hooks, since we need to know the actual target
 * socket, which is not looked up until we are inside the af_unix code.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_unix_stream_connect(struct sock *sock, struct sock *other,
				 struct sock *newsk)
/**
 * security_unix_may_send() - Check if AF_UNIX socket can send datagrams
 * @sock: originating sock
 * @other: peer sock
 *
 * Check permissions before connecting or sending datagrams from @sock to
 * @other.
 *
 * The @unix_stream_connect and @unix_may_send hooks were necessary because
 * Linux provides an alternative to the conventional file name space for Unix
 * domain sockets.  Whereas binding and connecting to sockets in the file name
 * space is mediated by the typical file permissions (and caught by the mknod
 * and permission hooks in inode_security_ops), binding and connecting to
 * sockets in the abstract name space is completely unmediated.  Sufficient
 * control of Unix domain sockets in the abstract name space isn't possible
 * using only the socket layer hooks, since we need to know the actual target
 * socket, which is not looked up until we are inside the af_unix code.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_create() - Check if creating a new socket is allowed
 * @family: protocol family
 * @type: communications type
 * @protocol: requested protocol
 * @kern: set to 1 if a kernel socket is requested
 *
 * Check permissions prior to creating a new socket.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_post_create() - Initialize a newly created socket
 * @sock: socket
 * @family: protocol family
 * @type: communications type
 * @protocol: requested protocol
 * @kern: set to 1 if a kernel socket is requested
 *
 * This hook allows a module to update or allocate a per-socket security
 * structure. Note that the security field was not added directly to the socket
 * structure, but rather, the socket security information is stored in the
 * associated inode.  Typically, the inode alloc_security hook will allocate
 * and attach security information to SOCK_INODE(sock)->i_security.  This hook
 * may be used to update the SOCK_INODE(sock)->i_security field with additional
 * information that wasn't available when the inode was allocated.
 *
 * Return: Returns 0 if permission is granted.
 */
			     protocol, kern);
/**
 * security_socket_socketpair() - Check if creating a socketpair is allowed
 * @socka: first socket
 * @sockb: second socket
 *
 * Check permissions before creating a fresh pair of sockets.
 *
 * Return: Returns 0 if permission is granted and the connection was
 *         established.
 */
/**
 * security_socket_bind() - Check if a socket bind operation is allowed
 * @sock: socket
 * @address: requested bind address
 * @addrlen: length of address
 *
 * Check permission before socket protocol layer bind operation is performed
 * and the socket @sock is bound to the address specified in the @address
 * parameter.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_socket_bind(struct socket *sock,
			 struct sockaddr *address, int addrlen)
{
	return call_int_hook(socket_bind, 0, sock, address, addrlen);
}
/**
 * security_socket_connect() - Check if a socket connect operation is allowed
 * @sock: socket
 * @address: address of remote connection point
 * @addrlen: length of address
 *
 * Check permission before socket protocol layer connect operation attempts to
 * connect socket @sock to a remote address, @address.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_socket_connect(struct socket *sock,
			    struct sockaddr *address, int addrlen)
/**
 * security_socket_listen() - Check if a socket is allowed to listen
 * @sock: socket
 * @backlog: connection queue size
 *
 * Check permission before socket protocol layer listen operation.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_accept() - Check if a socket is allowed to accept connections
 * @sock: listening socket
 * @newsock: newly creation connection socket
 *
 * Check permission before accepting a new connection.  Note that the new
 * socket, @newsock, has been created and some information copied to it, but
 * the accept operation has not actually been performed.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_sendmsg() - Check is sending a message is allowed
 * @sock: sending socket
 * @msg: message to send
 * @size: size of message
 *
 * Check permission before transmitting a message to another socket.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_recvmsg() - Check if receiving a message is allowed
 * @sock: receiving socket
 * @msg: message to receive
 * @size: size of message
 * @flags: operational flags
 *
 * Check permission before receiving a message from a socket.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_getsockname() - Check if reading the socket addr is allowed
 * @sock: socket
 *
 * Check permission before reading the local address (name) of the socket
 * object.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_getpeername() - Check if reading the peer's addr is allowed
 * @sock: socket
 *
 * Check permission before the remote address (name) of a socket object.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_getsockopt() - Check if reading a socket option is allowed
 * @sock: socket
 * @level: option's protocol level
 * @optname: option name
 *
 * Check permissions before retrieving the options associated with socket
 * @sock.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_setsockopt() - Check if setting a socket option is allowed
 * @sock: socket
 * @level: option's protocol level
 * @optname: option name
 *
 * Check permissions before setting the options associated with socket @sock.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_shutdown() - Checks if shutting down the socket is allowed
 * @sock: socket
 * @how: flag indicating how sends and receives are handled
 *
 * Checks permission before all or part of a connection on the socket @sock is
 * shut down.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sock_rcv_skb() - Check if an incoming network packet is allowed
 * @sk: destination sock
 * @skb: incoming packet
 *
 * Check permissions on incoming network packets.  This hook is distinct from
 * Netfilter's IP input hooks since it is the first time that the incoming
 * sk_buff @skb has been associated with a particular socket, @sk.  Must not
 * sleep inside this hook because some callers hold spinlocks.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_socket_getpeersec_stream() - Get the remote peer label
 * @sock: socket
 * @optval: destination buffer
 * @optlen: size of peer label copied into the buffer
 * @len: maximum size of the destination buffer
 *
 * This hook allows the security module to provide peer socket security state
 * for unix or connected tcp sockets to userspace via getsockopt SO_GETPEERSEC.
 * For tcp sockets this can be meaningful if the socket is associated with an
 * ipsec SA.
 *
 * Return: Returns 0 if all is well, otherwise, typical getsockopt return
 *         values.
 */
int security_socket_getpeersec_stream(struct socket *sock, sockptr_t optval,
				      sockptr_t optlen, unsigned int len)
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.socket_getpeersec_stream,
			     list) {
		rc = hp->hook.socket_getpeersec_stream(sock, optval, optlen,
						       len);
		if (rc != LSM_RET_DEFAULT(socket_getpeersec_stream))
			return rc;
	}
	return LSM_RET_DEFAULT(socket_getpeersec_stream);
/**
 * security_socket_getpeersec_dgram() - Get the remote peer label
 * @sock: socket
 * @skb: datagram packet
 * @secid: remote peer label secid
 *
 * This hook allows the security module to provide peer socket security state
 * for udp sockets on a per-packet basis to userspace via getsockopt
 * SO_GETPEERSEC. The application must first have indicated the IP_PASSSEC
 * option via getsockopt. It can then retrieve the security state returned by
 * this hook for a packet via the SCM_SECURITY ancillary message type.
 *
 * Return: Returns 0 on success, error on failure.
 */
int security_socket_getpeersec_dgram(struct socket *sock,
				     struct sk_buff *skb, u32 *secid)
	struct security_hook_list *hp;
	int rc;

	/*
	 * Only one module will provide a security context.
	 */
	hlist_for_each_entry(hp, &security_hook_heads.socket_getpeersec_dgram,
			     list) {
		rc = hp->hook.socket_getpeersec_dgram(sock, skb, secid);
		if (rc != LSM_RET_DEFAULT(socket_getpeersec_dgram))
			return rc;
	}
	return LSM_RET_DEFAULT(socket_getpeersec_dgram);
/**
 * security_sk_alloc() - Allocate and initialize a sock's LSM blob
 * @sk: sock
 * @family: protocol family
 * @priority: gfp flags
 *
 * Allocate and attach a security structure to the sk->sk_security field, which
 * is used to copy security attributes between local stream sockets.
 *
 * Return: Returns 0 on success, error on failure.
 */
/**
 * security_sk_free() - Free the sock's LSM blob
 * @sk: sock
 *
 * Deallocate security structure.
 */
/**
 * security_sk_clone() - Clone a sock's LSM state
 * @sk: original sock
 * @newsk: target sock
 *
 * Clone/copy security structure.
 */
/**
 * security_sk_classify_flow() - Set a flow's secid based on socket
 * @sk: original socket
 * @flic: target flow
 *
 * Set the target flow's secid to socket's secid.
 */
void security_sk_classify_flow(const struct sock *sk, struct flowi_common *flic)
/**
 * security_req_classify_flow() - Set a flow's secid based on request_sock
 * @req: request_sock
 * @flic: target flow
 *
 * Sets @flic's secid to @req's secid.
 */
/**
 * security_sock_graft() - Reconcile LSM state when grafting a sock on a socket
 * @sk: sock being grafted
 * @parent: target parent socket
 *
 * Sets @parent's inode secid to @sk's secid and update @sk with any necessary
 * LSM state from @parent.
 */
/**
 * security_inet_conn_request() - Set request_sock state using incoming connect
 * @sk: parent listening sock
 * @skb: incoming connection
 * @req: new request_sock
 *
 * Initialize the @req LSM state based on @sk and the incoming connect in @skb.
 *
 * Return: Returns 0 if permission is granted.
 */
			       struct sk_buff *skb, struct request_sock *req)
/**
 * security_inet_csk_clone() - Set new sock LSM state based on request_sock
 * @newsk: new sock
 * @req: connection request_sock
 *
 * Set that LSM state of @sock using the LSM state from @req.
 */
			     const struct request_sock *req)
/**
 * security_inet_conn_established() - Update sock's LSM state with connection
 * @sk: sock
 * @skb: connection packet
 *
 * Update @sock's LSM state to represent a new connection from @skb.
 */
				    struct sk_buff *skb)
/**
 * security_secmark_relabel_packet() - Check if setting a secmark is allowed
 * @secid: new secmark value
 *
 * Check if the process should be allowed to relabel packets to @secid.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_secmark_refcount_inc() - Increment the secmark labeling rule count
 *
 * Tells the LSM to increment the number of secmark labeling rules loaded.
 */
/**
 * security_secmark_refcount_dec() - Decrement the secmark labeling rule count
 *
 * Tells the LSM to decrement the number of secmark labeling rules loaded.
 */
/**
 * security_tun_dev_alloc_security() - Allocate a LSM blob for a TUN device
 * @security: pointer to the LSM blob
 *
 * This hook allows a module to allocate a security structure for a TUN	device,
 * returning the pointer in @security.
 *
 * Return: Returns a zero on success, negative values on failure.
 */
/**
 * security_tun_dev_free_security() - Free a TUN device LSM blob
 * @security: LSM blob
 *
 * This hook allows a module to free the security structure for a TUN device.
 */
/**
 * security_tun_dev_create() - Check if creating a TUN device is allowed
 *
 * Check permissions prior to creating a new TUN device.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_tun_dev_attach_queue() - Check if attaching a TUN queue is allowed
 * @security: TUN device LSM blob
 *
 * Check permissions prior to attaching to a TUN device queue.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_tun_dev_attach() - Update TUN device LSM state on attach
 * @sk: associated sock
 * @security: TUN device LSM blob
 *
 * This hook can be used by the module to update any security state associated
 * with the TUN device's sock structure.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_tun_dev_open() - Update TUN device LSM state on open
 * @security: TUN device LSM blob
 *
 * This hook can be used by the module to update any security state associated
 * with the TUN device's security structure.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_sctp_assoc_request() - Update the LSM on a SCTP association req
 * @asoc: SCTP association
 * @skb: packet requesting the association
 *
 * Passes the @asoc and @chunk->skb of the association INIT packet to the LSM.
 *
 * Return: Returns 0 on success, error on failure.
 */
int security_sctp_assoc_request(struct sctp_association *asoc,
				struct sk_buff *skb)
/**
 * security_sctp_bind_connect() - Validate a list of addrs for a SCTP option
 * @sk: socket
 * @optname: SCTP option to validate
 * @address: list of IP addresses to validate
 * @addrlen: length of the address list
 *
 * Validiate permissions required for each address associated with sock	@sk.
 * Depending on @optname, the addresses will be treated as either a connect or
 * bind service. The @addrlen is calculated on each IPv4 and IPv6 address using
 * sizeof(struct sockaddr_in) or sizeof(struct sockaddr_in6).
 *
 * Return: Returns 0 on success, error on failure.
 */
/**
 * security_sctp_sk_clone() - Clone a SCTP sock's LSM state
 * @asoc: SCTP association
 * @sk: original sock
 * @newsk: target sock
 *
 * Called whenever a new socket is created by accept(2) (i.e. a TCP style
 * socket) or when a socket is 'peeled off' e.g userspace calls
 * sctp_peeloff(3).
 */
/**
 * security_sctp_assoc_established() - Update LSM state when assoc established
 * @asoc: SCTP association
 * @skb: packet establishing the association
 *
 * Passes the @asoc and @chunk->skb of the association COOKIE_ACK packet to the
 * security module.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_mptcp_add_subflow() - Inherit the LSM label from the MPTCP socket
 * @sk: the owning MPTCP socket
 * @ssk: the new subflow
 *
 * Update the labeling for the given MPTCP subflow, to match the one of the
 * owning MPTCP socket. This hook has to be called after the socket creation and
 * initialization via the security_socket_create() and
 * security_socket_post_create() LSM hooks.
 *
 * Return: Returns 0 on success or a negative error code on failure.
 */
int security_mptcp_add_subflow(struct sock *sk, struct sock *ssk)
{
	return call_int_hook(mptcp_add_subflow, 0, sk, ssk);
}

/**
 * security_ib_pkey_access() - Check if access to an IB pkey is allowed
 * @sec: LSM blob
 * @subnet_prefix: subnet prefix of the port
 * @pkey: IB pkey
 *
 * Check permission to access a pkey when modifying a QP.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_ib_endport_manage_subnet() - Check if SMPs traffic is allowed
 * @sec: LSM blob
 * @dev_name: IB device name
 * @port_num: port number
 *
 * Check permissions to send and receive SMPs on a end port.
 *
 * Return: Returns 0 if permission is granted.
 */
int security_ib_endport_manage_subnet(void *sec,
				      const char *dev_name, u8 port_num)
	return call_int_hook(ib_endport_manage_subnet, 0, sec,
			     dev_name, port_num);
/**
 * security_ib_alloc_security() - Allocate an Infiniband LSM blob
 * @sec: LSM blob
 *
 * Allocate a security structure for Infiniband objects.
 *
 * Return: Returns 0 on success, non-zero on failure.
 */
/**
 * security_ib_free_security() - Free an Infiniband LSM blob
 * @sec: LSM blob
 *
 * Deallocate an Infiniband security structure.
 */
/**
 * security_xfrm_policy_alloc() - Allocate a xfrm policy LSM blob
 * @ctxp: xfrm security context being added to the SPD
 * @sec_ctx: security label provided by userspace
 * @gfp: gfp flags
 *
 * Allocate a security structure to the xp->security field; the security field
 * is initialized to NULL when the xfrm_policy is allocated.
 *
 * Return:  Return 0 if operation was successful.
 */
/**
 * security_xfrm_policy_clone() - Clone xfrm policy LSM state
 * @old_ctx: xfrm security context
 * @new_ctxp: target xfrm security context
 *
 * Allocate a security structure in new_ctxp that contains the information from
 * the old_ctx structure.
 *
 * Return: Return 0 if operation was successful.
 */
			       struct xfrm_sec_ctx **new_ctxp)
/**
 * security_xfrm_policy_free() - Free a xfrm security context
 * @ctx: xfrm security context
 *
 * Free LSM resources associated with @ctx.
 */
/**
 * security_xfrm_policy_delete() - Check if deleting a xfrm policy is allowed
 * @ctx: xfrm security context
 *
 * Authorize deletion of a SPD entry.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_xfrm_state_alloc() - Allocate a xfrm state LSM blob
 * @x: xfrm state being added to the SAD
 * @sec_ctx: security label provided by userspace
 *
 * Allocate a security structure to the @x->security field; the security field
 * is initialized to NULL when the xfrm_state is allocated. Set the context to
 * correspond to @sec_ctx.
 *
 * Return: Return 0 if operation was successful.
 */
/**
 * security_xfrm_state_alloc_acquire() - Allocate a xfrm state LSM blob
 * @x: xfrm state being added to the SAD
 * @polsec: associated policy's security context
 * @secid: secid from the flow
 *
 * Allocate a security structure to the x->security field; the security field
 * is initialized to NULL when the xfrm_state is allocated.  Set the context to
 * correspond to secid.
 *
 * Return: Returns 0 if operation was successful.
 */
/**
 * security_xfrm_state_delete() - Check if deleting a xfrm state is allowed
 * @x: xfrm state
 *
 * Authorize deletion of x->security.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_xfrm_state_free() - Free a xfrm state
 * @x: xfrm state
 *
 * Deallocate x->security.
 */
/**
 * security_xfrm_policy_lookup() - Check if using a xfrm policy is allowed
 * @ctx: target xfrm security context
 * @fl_secid: flow secid used to authorize access
 *
 * Check permission when a flow selects a xfrm_policy for processing XFRMs on a
 * packet.  The hook is called when selecting either a per-socket policy or a
 * generic xfrm policy.
 *
 * Return: Return 0 if permission is granted, -ESRCH otherwise, or -errno on
 *         other errors.
 */
/**
 * security_xfrm_state_pol_flow_match() - Check for a xfrm match
 * @x: xfrm state to match
 * @xp: xfrm policy to check for a match
 * @flic: flow to check for a match.
 *
 * Check @xp and @flic for a match with @x.
 *
 * Return: Returns 1 if there is a match.
 */
			     list) {
/**
 * security_xfrm_decode_session() - Determine the xfrm secid for a packet
 * @skb: xfrm packet
 * @secid: secid
 *
 * Decode the packet in @skb and return the security label in @secid.
 *
 * Return: Return 0 if all xfrms used have the same secid.
 */
			       0);
/**
 * security_key_alloc() - Allocate and initialize a kernel key LSM blob
 * @key: key
 * @cred: credentials
 * @flags: allocation flags
 *
 * Permit allocation of a key and assign security data. Note that key does not
 * have a serial number assigned at this point.
 *
 * Return: Return 0 if permission is granted, -ve error otherwise.
 */
/**
 * security_key_free() - Free a kernel key LSM blob
 * @key: key
 *
 * Notification of destruction; free security data.
 */
/**
 * security_key_permission() - Check if a kernel key operation is allowed
 * @key_ref: key reference
 * @cred: credentials of actor requesting access
 * @need_perm: requested permissions
 *
 * See whether a specific operational right is granted to a process on a key.
 *
 * Return: Return 0 if permission is granted, -ve error otherwise.
 */
/**
 * security_key_getsecurity() - Get the key's security label
 * @key: key
 * @buffer: security label buffer
 *
 * Get a textual representation of the security context attached to a key for
 * the purposes of honouring KEYCTL_GETSECURITY.  This function allocates the
 * storage for the NUL-terminated string and the caller should free it.
 *
 * Return: Returns the length of @buffer (including terminating NUL) or -ve if
 *         an error occurs.  May also return 0 (and a NULL buffer pointer) if
 *         there is no security label assigned to the key.
 */
int security_key_getsecurity(struct key *key, char **buffer)
	*buffer = NULL;
	return call_int_hook(key_getsecurity, 0, key, buffer);
/**
 * security_audit_rule_init() - Allocate and init an LSM audit rule struct
 * @field: audit action
 * @op: rule operator
 * @rulestr: rule context
 * @lsmrule: receive buffer for audit rule struct
 *
 * Allocate and initialize an LSM audit rule structure.
 *
 * Return: Return 0 if @lsmrule has been successfully set, -EINVAL in case of
 *         an invalid rule.
 */
/**
 * security_audit_rule_known() - Check if an audit rule contains LSM fields
 * @krule: audit rule
 *
 * Specifies whether given @krule contains any fields related to the current
 * LSM.
 *
 * Return: Returns 1 in case of relation found, 0 otherwise.
 */
/**
 * security_audit_rule_free() - Free an LSM audit rule struct
 * @lsmrule: audit rule struct
 *
 * Deallocate the LSM audit rule structure previously allocated by
 * audit_rule_init().
 */
/**
 * security_audit_rule_match() - Check if a label matches an audit rule
 * @secid: security label
 * @field: LSM audit field
 * @op: matching operator
 * @lsmrule: audit rule
 *
 * Determine if given @secid matches a rule previously approved by
 * security_audit_rule_known().
 *
 * Return: Returns 1 if secid matches the rule, 0 if it does not, -ERRNO on
 *         failure.
 */
/**
 * security_bpf() - Check if the bpf syscall operation is allowed
 * @cmd: command
 * @attr: bpf attribute
 * @size: size
 *
 * Do a initial check for all bpf syscalls after the attribute is copied into
 * the kernel. The actual security module can implement their own rules to
 * check the specific cmd they need.
 *
 * Return: Returns 0 if permission is granted.
 */

/**
 * security_bpf_map() - Check if access to a bpf map is allowed
 * @map: bpf map
 * @fmode: mode
 *
 * Do a check when the kernel generates and returns a file descriptor for eBPF
 * maps.
 *
 * Return: Returns 0 if permission is granted.
 */

/**
 * security_bpf_prog() - Check if access to a bpf program is allowed
 * @prog: bpf program
 *
 * Do a check when the kernel generates and returns a file descriptor for eBPF
 * programs.
 *
 * Return: Returns 0 if permission is granted.
 */

/**
 * security_bpf_map_alloc() - Allocate a bpf map LSM blob
 * @map: bpf map
 *
 * Initialize the security field inside bpf map.
 *
 * Return: Returns 0 on success, error on failure.
 */

/**
 * security_bpf_prog_alloc() - Allocate a bpf program LSM blob
 * @aux: bpf program aux info struct
 *
 * Initialize the security field inside bpf program.
 *
 * Return: Returns 0 on success, error on failure.
 */

/**
 * security_bpf_map_free() - Free a bpf map's LSM blob
 * @map: bpf map
 *
 * Clean up the security information stored inside bpf map.
 */

/**
 * security_bpf_prog_free() - Free a bpf program's LSM blob
 * @aux: bpf program aux info struct
 *
 * Clean up the security information stored inside bpf prog.
 */
/**
 * security_locked_down() - Check if a kernel feature is allowed
 * @what: requested kernel feature
 *
 * Determine whether a kernel feature that potentially enables arbitrary code
 * execution in kernel space should be permitted.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_perf_event_open() - Check if a perf event open is allowed
 * @attr: perf event attribute
 * @type: type of event
 *
 * Check whether the @type of perf_event_open syscall is allowed.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_perf_event_alloc() - Allocate a perf event LSM blob
 * @event: perf event
 *
 * Allocate and save perf_event security info.
 *
 * Return: Returns 0 on success, error on failure.
 */
/**
 * security_perf_event_free() - Free a perf event LSM blob
 * @event: perf event
 *
 * Release (free) perf_event security info.
 */
/**
 * security_perf_event_read() - Check if reading a perf event label is allowed
 * @event: perf event
 *
 * Read perf_event security info if allowed.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_perf_event_write() - Check if writing a perf event label is allowed
 * @event: perf event
 *
 * Write perf_event security info if allowed.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_uring_override_creds() - Check if overriding creds is allowed
 * @new: new credentials
 *
 * Check if the current task, executing an io_uring operation, is allowed to
 * override it's credentials with @new.
 *
 * Return: Returns 0 if permission is granted.
 */
/**
 * security_uring_sqpoll() - Check if IORING_SETUP_SQPOLL is allowed
 *
 * Check whether the current task is allowed to spawn a io_uring polling thread
 * (IORING_SETUP_SQPOLL).
 *
 * Return: Returns 0 if permission is granted.
 */

/**
 * security_uring_cmd() - Check if a io_uring passthrough command is allowed
 * @ioucmd: command
 *
 * Check whether the file_operations uring_cmd is allowed to run.
 *
 * Return: Returns 0 if permission is granted.
 */