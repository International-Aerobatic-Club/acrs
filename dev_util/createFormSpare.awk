BEGIN {
    IDPrefix="input";
    startgroup=true;
    FS=","; 
    print "<html><head><style>label {display:block}</style></head><body>";
    print "<form class=\"recordForm\" action=\"$action\" method=\"post\">";
    }
END {
    print "<div class=\"submit\">";
    print "<input class=\"submit\" name=\"submit\" type=\"submit\" value=\"$submitText\"/>";
    print "</div>";
    print "</form>";
    print "</body></html>";
    }
/select/ {
    print "<label for=\"" IDPrefix NR "\">" $2 ":</label><fieldset id=\"" IDPrefix NR "\" legend=\"" $2 "\">";
    for(i=4;i<=NF;i+=1)
    {
      print "<input class=\"form_select\" type=\"radio\" name=\"" $3 "\" value=\"" $i "\".isSelected($record,\"" $3 "\",\"" $i "\")>" $i "</input>";
    }
    print "</fieldset>";
    }
/check/ {
    print "<label for=\"" IDPrefix NR "\">" $2 ":</label><fieldset id=\"" IDPrefix NR "\" legend=\"" $2 "\">";
    for(i=4;i<=NF;i+=1)
    {
      print "<input class=\"form_check\" type=\"checkbox\" name=\"" $3 "_" $i "\" .isChecked($record,\"" $3 "\",\"" $i "\")>" $i "</input>";
    }  
    print "</fieldset>";
    }
/int/ {
    print "<label for=\"" IDPrefix NR "\">" $2 ":</label><input id=\"" IDPrefix NR "\" name=\"" $3 "\" value=\"$record[\"" $3 "\"]\" maxlength=\"" $6 "\" size=\"" $5 "\"/>";
    }
/text/ {
    print "<label for=\"" IDPrefix NR "\">" $2 ":</label><input id=\"" IDPrefix NR "\" name=\"" $3 "\" value=\"$record[\"" $3 "\"]\" maxlength=\"" $6 "\" size=\"" $5 "\"/>";
    }
/boolean/ {
    print "<input class=\"form_boolean\" type=\"checkbox\" name=\"" $3 "\" boolChecked($record,\"" $3 "\")>" $2 "</input>";
    }
