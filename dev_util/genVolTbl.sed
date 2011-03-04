# converts html output of genVolTbl.awk into php
# the awk script places php variable and function 
# call elements within comments
1i<?php function isCompCat($volunteer, $cat) { return ($cat == 'advanced'); }
1ifunction roleChecked($volunteer, $cat, $role) { return false; }
s/"/\\"/g #escape quotes
s/^/echo "/ #prepend echo command
s/$/\\n";/ #append end of string, newline, semicolon
s/echo "<!--\[//g #eliminate comments around code
s/\]-->\\n";//g #eliminate comments around code
s/<!--/"\./g #translate start of comment into concat
s/-->/\."/g #translate end of comment into concat
$a?>
