# converts html output of createForm.awk into php
# the awk script places php variable and function 
# call elements within comments, with single quotes
# where the form needs php quotes.
1i<?php
s/"/\\"/g #escape quotes
s/^/echo "/ #prepend echo command
s/$/\\n";/ #append end of string, newline, semicolon
s/<!--/"\./g #translate start of comment into concat
s/-->/\."/g #translate end of comment into concat
$a?>
