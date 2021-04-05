!                       Input.h
! Use this file to get all the inputs from the PRM file
!

!#include 'input.{targetlanguage}'
#Include 'multschm.h'
!#Include '\\eugras135\c\sqr6\reports\multschm.h'

begin-procedure get-input

input $where   {whereprompt}

if {p1type} = 'DATE'
input $P1      {p1prompt} type=DATE format=DATE Status=#p1_status
else
input $P1      {p1prompt}
end-if

if {p2type} = 'DATE'
input $P2      {p2prompt} type=DATE format=DATE Status=#p2_status
else
input $P2      {p2prompt}
end-if

if {p3type} = 'DATE'
input $P3      {p3prompt} type=DATE format=DATE Status=#p3_status
else
input $P3      {p3prompt}
end-if

if {p4type} = 'DATE'
input $P4      {p4prompt} type=DATE format=DATE Status=#p4_status
else
input $P4      {p4prompt}
end-if

input $schema  {schemaprompt}

	! modify the where if needed
	do GetSchema( $schema, $where )

    do set-security ( $schema )

end-procedure


begin-procedure set-security ( $schema )

begin-sql
begin
[$schema]BPSECURITY_CTX.SET_SECURITY( $_username ,'SQRW');;
DBMS_APPLICATION_INFO.SET_CLIENT_INFO( $_username||'@SQRW' );;
end;;
end-sql

end-procedure





!--------------- Check user is in a valid group. If no entry in maxvars fro report assume user is ok -----
Begin-procedure user_check
let $groups=''
let $users=''
Begin-select
GROUPS          &groups
USERS           &users
 move &groups to $groups
 move &users to  $users
from [$schema]BPREPRES where REPORT=$report_name
End-select


if $groups<>''
! There is an entry in REPREST  for report and some groups are valid
  let $userok='N'                                    ! assume user is not valid
  let #totlength = length($groups)                   ! set the total length of all groups ($groups)
  let #char=0                                        ! position in $groups
  let #start=1                                       ! starting position of group
  while 1 = 1
    let #char = #char + 1                            ! move pointer 1 character to right in $groups
    if #char > #totlength                            ! end of groups
      break
    end-if
    if substr($groups,#char,1)=','                   ! found a comma
      let #length = #char-#start                     ! work out length of group code
      let $groupcode=substr($groups,#start,#length)  ! extract group code from $groups
! ----- find if user and group are set up in the MAXUSERGROUPS table ----------
 uppercase $username
 uppercase $groupcode

begin-select
usrname   &usr1
  let $userok='Y'                                    ! set the flag to indicate user is valid
from [$schema]maxusergroups
where usrname = $username
and grpname = $groupcode
end-select
        let #start = #char + 1                       ! position the new start of group code to 1 past the comma
    end-if
  end-while
! Need to process the last group code which is after the last comma in $groups
  let #length = #char-#start                         ! work out length of group code
  let $groupcode=substr($groups,#start,#length)      ! extract group code from $groups

! ----- find if user and group are set up in the MAXUSERGROUPS table ----------
uppercase $username
uppercase $groupcode
begin-select
usrname  &usr2
 let $userok='Y'                                     ! set the flag to indicate user is valid
from [$schema]maxusergroups
where usrname = $username
and grpname = $groupcode
end-select
end-if


! ---- User is not part of a valid group, check for them being a valid user -------
if $userok<>'Y' and $users<>''
! There is an entry in REPREST  for report and some users are valid
  let $userok='N'                                    ! assume user is not valid
  let #totlength = length($users)                   ! set the total length of all users ($users)
  let #char=0                                        ! position in $users
  let #start=1                                       ! starting position of user
  while 1 = 1
    let #char = #char + 1                            ! move pointer 1 character to right in $users
    if #char > #totlength                            ! end of users
      break
    end-if
    if substr($users,#char,1)=','                   ! found a comma
      let #length = #char-#start                     ! work out length of user code
      let $usercode=substr($users,#start,#length)  ! extract user code from $users
! ----- find if $usercode is the same as the user extracted ----------
      if upper($username)=upper($usercode)
       let $userok='Y'                                    ! set the flag to indicate user is valid
      end-if
      let #start = #char + 1                       ! position the new start of user code to 1 past the comma
    end-if
  end-while
! Need to process the last user code which is after the last comma in $users
  let #length = #char-#start                         ! work out length of user code
  let $usercode=substr($users,#start,#length)      ! extract user code from $users
! ----- find if $usercode is the same as the user extracted ----------
  if upper($username)=upper($usercode)
   let $userok='Y'                                    ! set the flag to indicate user is valid
  end-if
end-if
End-procedure user_check

