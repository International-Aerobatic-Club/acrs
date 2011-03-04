BEGIN {
    FS=","; 
    print "require_once('validMMDD.php');";
    print "require_once('validInt.php');";
    print "function validatePost(& $record)";
    print "{";
    print "$corrMsg = '';";
    }
END {
    print "return $corrMsg;";
    print "}";
    }
/int/ {
    print "$fail = validInt($record, '" $3 "', '" $2 "', " $6 ", true);";
    print "if ($fail != '') $corrMsg .= '<li>' . $fail . '</li>';";
    }
/text/ {
    print "$record['" $3 "'] = crop($record['" $3 "'], " $6 ");";
    print "if (strlen($record['" $3 "']) == 0)";
    print "{";
    print "   $corrMsg .= '<li>Provide " $2 "</li>';";
    print "}";
    }
/bool/ {
    print "if (boolChecked($record, '" $3 "'));";
    }
/yesno/ {
    print "if (boolChecked($record, '" $3 "'));";
    }
/mmdd/ {
    print "$fail = validMMDD($record['" $3 "'], $record['" $4 "'], '" $2 "');";
    print "if ($fail != '') $corrMsg .= '<li>' . $fail . '</li>';";
    }
/hhmm/ {
    print "$fail = validHHMM($record['" $3 "'], '" $2 "');";
    print "if ($fail != '') $corrMsg .= '<li>' . $fail . '</li>';";
    }
