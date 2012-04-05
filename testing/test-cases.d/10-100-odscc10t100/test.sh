#!/usr/bin/env bash
#
# Use a Repository Capacity of 0 and expect failure

if [ -n "$HAVE_MYSQL" ]; then
	ods_setup_conf conf.xml conf-mysql.xml
fi &&

! ods_reset_env &&
log_grep ods-ksmutil-setup stderr "Type positiveInteger doesn't allow value '0'" &&

! log_this ods-control-enforcer-start ods-control enforcer start &&
syslog_waitfor 10 "ods-enforcerd: .*Type positiveInteger doesn't allow value '0'" &&
! pgrep 'ods-enforcerd' >/dev/null 2>/dev/null &&
return 0

ods_kill
return 1
