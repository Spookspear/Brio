begin-procedure GetSchema( :$schema, :$where )

!declare-variable
!	integer #Param
!	integer #TotalLen
!	integer #TablePos
!	integer #Pos1
!	integer #Pos2
!end-declare

	! re-organize $where clause, if the where clause has a sub-select,
	! we need to put the schema in front of the tabel.
	move 1 to #where-sel

	! use loop here is because we dont know how many sub-select in the where clause yet.
	while #where-sel

		let #where-sel = instr( lower($where), 'select', 1 )
	
		! if there has sub-select in the where clause
		If #where-sel

			let #TotalLen = length( $where )
			let #TablePos = instr( lower($where), 'from', 1 )

			move #TablePos to #Pos1
			move #TablePos to #Pos2
			let #Pos1 = #Pos1 + 4
			let #Pos2 = #Pos2 + 5

			subtract #TablePos from #TotalLen
			let $where1 = substr( $where, 0, #Pos1 )
			let $where2 = substr( $where, #Pos2, #TotalLen )

			! put $where2 back to $where for another sub-select check.
			string $schema $where2 by '' into $where

			! concatnate re-organized string to a temporary string.
			concat $where1 with $SchemaWhere
			concat ' ' with $SchemaWhere
		
		else
	
			! if no more sub-select found, put everything together to the where clause.
			string $SchemaWhere $where by ' ' into $where

		end-if

	end-while


end-procedure
