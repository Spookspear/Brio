!                       Input.h
! Use this file to get all the inputs From the PRM file
!

!#include 'input.{targetlanguage}'
#Include 'multschm.h'
!#Include '\\eugras135\c\sqr6\reports\multschm.h'

Begin-Procedure get-input

    Input $Where   {Whereprompt}

    If {p1type} = 'DATE'
        Input $P1      {p1prompt} type=DATE format=DATE Status=#p1_status
    Else
        Input $P1      {p1prompt}
    End-If

    If {p2type} = 'DATE'
        Input $P2      {p2prompt} type=DATE format=DATE Status=#p2_status
    Else
        Input $P2      {p2prompt}
    End-If

    If {p3type} = 'DATE'
        Input $P3      {p3prompt} type=DATE format=DATE Status=#p3_status
    Else
        Input $P3      {p3prompt}
    End-If

    If {p4type} = 'DATE'
        Input $P4      {p4prompt} type=DATE format=DATE Status=#p4_status
    Else
        Input $P4      {p4prompt}
    End-If

    Input $schema  {schemaprompt}

    ! modify the Where If needed
    Do GetSchema( $schema, $Where )
    Do set-security ( $schema )

End-Procedure


Begin-Procedure set-security ( $schema )

    Begin-Sql
        Begin
            BPSECURITY_CTX.SET_SECURITY( $_username ,'SQRW');;
            DBMS_APPLICATION_INFO.SET_CLIENT_INFO( $_username||'@SQRW' );;
        End;;
    End-Sql

End-Procedure


!--------------- Check user is in a valid group. If no entry in maxvars fro report assume user is ok -----
Begin-Procedure user_check
    Let $groups=''
    Let $users=''

    Begin-Select
        GROUPS          &groups
        USERS           &users
        Move &groups to $groups
        Move &users to  $users
        From BPREPRES Where REPORT=$report_name
    End-Select


    If $groups<>''

        ! There is an entry in REPREST  for report And some groups are valid
        Let $userok='N'                                         ! assume user is not valid
        Let #totlength = Length($groups)                        ! Set the total length of all groups ($groups)
        Let #char=0                                             ! Position in $groups
        Let #start=1                                            ! starting Position of group

        While 1 = 1
            Let #char = #char + 1                               ! Move pointer 1 character to right in $groups
            If #char > #totlength                               ! End of groups
                Break
            End-If

            If SubStr($groups,#char,1)=','                      ! found a comma
                Let #length = #char-#start                      ! work out length of group code
                Let $groupcode=SubStr($groups,#start,#length)   ! extract group code From $groups

                ! ----- find If user And group are Set up in the MAXUSERGROUPS table ----------
                 UpperCase $username
                 UpperCase $groupcode

                Begin-Select
                    USRNAME   &usr1
                    Let $userok='Y'                             ! Set the flag to indicate user is valid
                    From MAXUSERGROUPS
                    Where USRNAME = $username
                    And GRPNAME = $groupcode
                End-Select

                Let #start = #char + 1                          ! Position the new start of group code to 1 past the comma

            End-If

        End-While

        ! Need to process the last group code which is after the last comma in $groups
        Let #length = #char-#start                              ! work out length of group code
        Let $groupcode=SubStr($groups,#start,#length)           ! extract group code From $groups

        ! ----- find If user And group are Set up in the MAXUSERGROUPS table ----------

        UpperCase $username
        UpperCase $groupcode

        Begin-Select
            usrname  &usr2
            Let $userok='Y'                                     ! Set the flag to indicate user is valid
            From MAXUSERGROUPS
            Where USRNAME = $username
            And GRPNAME = $groupcode
        End-Select

    End-If

    ! ---- User is not part of a valid group, check for them being a valid user -------
    If $userok<>'Y' And $users<>''
        ! There is an entry in REPREST  for report And some users are valid
        Let $userok='N'                                         ! assume user is not valid
        Let #totlength = Length($users)                         ! Set the total length of all users ($users)
        Let #char=0                                             ! Position in $users
        Let #start=1                                            ! starting Position of user

        While 1 = 1
        Let #char = #char + 1                                   ! Move pointer 1 character to right in $users
            If #char > #totlength                               ! End of users
                Break
            End-If

            If SubStr($users,#char,1)=','                       ! found a comma
                Let #length = #char-#start                      ! work out length of user code
                Let $usercode=SubStr($users,#start,#length)     ! extract user code From $users

                ! ----- find If $usercode is the same as the user extracted ----------
                If Upper($username)=Upper($usercode)
                    Let $userok='Y'                             ! Set the flag to indicate user is valid
                End-If

                Let #start = #char + 1                          ! Position the new start of user code to 1 past the comma
            End-If
        End-While

        ! Need to process the last user code which is after the last comma in $users
        Let #length = #char-#start                              ! work out length of user code
        Let $usercode=SubStr($users,#start,#length)             ! extract user code From $users

        ! ----- find If $usercode is the same as the user extracted ----------
        If Upper($username)=Upper($usercode)
            Let $userok='Y'                                     ! Set the flag to indicate user is valid
        End-If
    End-If

End-Procedure user_check

