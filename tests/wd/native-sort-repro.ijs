NB. Run in a fresh JQt session built from the native-table-row-identity branch.
NB. Click Key repeatedly. Each label, ID and value should stay together.
wd 'pc rowsort;pn "Native row identity";bin v'
wd 'cc pac table 5 3;pas 0 0'
wd 'set pac hdr Key ID Value;set pac lab rowB rowA1 rowA2 rowEmpty rowC'
wd 'set pac data B idB valueB A idA1 valueA1 A idA2 valueA2 "" idEmpty empty C idC valueC'
wd 'set pac celltip 0 1 tip-for-idB'
wd 'set pac block 0 2;set pac type 100;set pac cell 0 2 1'
wd 'set pac block 1 2;set pac type 200;set pac cell 1 2 "0 red green"'
wd 'set pac block 2 2;set pac type 400;set pac cell 2 2 Push'
wd 'set pac block;set pac headerclick native;pshow'
NB. After each sort, clicking checkbox/combo/button must report their CURRENT
NB. displayed row (zero-based) in the J session; click an ordinary cell first
NB. if you also want to observe selection/current-cell movement.
rowsort_pac_change=: 3 :0
smoutput 'change cell=',pac_cell,' value=',pac_value
)
rowsort_pac_clicked=: 3 :0
smoutput 'button cell=',pac_cell
)
rowsort_pac_hdrclick=: 3 :0
smoutput 'header=',pac_hdrcol,' sort=',pac_sortind
)
NB. To compare event mode (arrow/events only, no reorder):
NB. wd 'set pac headerclick event'
