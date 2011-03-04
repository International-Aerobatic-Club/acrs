BEGIN {
    FS=","; 
    OFS=" ";
    ORS="";
    }
function showSet()
{
    for (i = 4; i <= NF; ++i)
    {
       if (4 < i) print ", ";
       print "'" $i "'";
    }
}
/text/ {
    print "$update .= '" $3 " = '.strSQL($record['" $3 "']," $6 ").',';\n";
    }
/select/ {
    print "$_enumSet_" $3 " = array(";
    showSet();
    print ");\n";
    print "$update .= '" $3 " = '.enumSQL($record['" $3 "'], $_enumSet_" $3 ").',';\n";
    }
/check/ {
    print "$_enumSet_" $3 " = array(";
    showSet();
    print ");\n";
    print "$update .= '" $3 "= '.selectionSQL($record, '" $3 "', $_enumSet_" $3 ").',';\n";
    }
/boolean/ {
    print "$update .= '" $3 "= '.boolSQL($record, '" $3 "').',';\n";
    }
/int/ {
    print "$update .= '" $3 "= '.intSQL($record['" $3 "']).',';\n";
    }
/yesno/ {
    print "$update .= '" $3 "= '.boolSQL($record, '" $3 "').',';\n";
    }
/mmdd/ {
    print "$update .= '" $3 "= '.dateSQL($record['" $3 "']).',';\n";
    }
/hhmm/ {
    print "$update .= '" $3 "= '.dateSQL($record['" $3 "']).',';\n";
    }
/date/ {
    print "$update .= '" $3 "= '.dateSQL($record['" $3 "']).',';\n";
    }
