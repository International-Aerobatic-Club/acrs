BEGIN {
    FS=","; 
    OFS=" ";
    ORS="";
    print "$record = dbFetchAssoc($result);\n";
    }
/select/ {
    print "sqlSetValueToPostData($record['" $3 "'], '" $3 "', $record);\n";
    }
/bool/ {
    print "sqlBoolValueToPostData($record['" $3 "'], '" $3 "', $record);\n";
    }
/yesno/ {
    print "sqlBoolValueToPostData($record['" $3 "'], '" $3 "', $record);\n";
    }
/mmdd/ {
    print "$date = strtotime($record['" $3 "']);\n"
    print "$record['" $3 "'] = strftime('%m/%d', $date);\n"
    }
/hhmm/ {
    print "$time = strtotime($record['" $3 "']);\n"
    print "$record['" $3 "'] = strftime('%H:%M', $time);\n"
    }
