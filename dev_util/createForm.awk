# Create form from data item description
# separate fields with commas
# field 1: datatype.  one from set, {select, check, int, text, boolean}.
# field 2: printable name of data item
# field 3: data item column name
# for type select, remaining fields are possible values (user selects one)
# for type check, remaining fields are possible values (user selects zero or more)
# for type int: 
#    field 4: sql integer type
#    field 5: size of input field
#    field 6: max length of integer string
# for type text
#    field 4: sql text type (char, varchar)
#    field 5: size of input field
#    field 6: max length of input
# for type boolean, no additional fields required
# for type yesno, no additional fields required
# for type mmdd: 
#    field 4: name of field that contains year
BEGIN {
    IDPrefix="in";
    startgroup=true;
    FS=","; 
    print "<html><head>";
    print "<link href=\"regform.css\" type=\"text/css\" rel=\"stylesheet\"/>";
    print "<style>label {display:block}</style>";
    print "</head><body>";
    print "<form class=\"recordForm\" action=\"<!--$action-->\" method=\"post\">";
    print "<table><tbody>";
    print "<tr>";
    }
END {
    print "</tr>";
    print "</tbody></table>";
    print "<div class=\"submit\">";
    print "<input class=\"submit\" name=\"submit\" type=\"submit\" value=\"<!--$submitText-->\"/>";
    print "</div>";
    print "</form>";
    print "</body></html>";
    }
/select/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_select\"><label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><fieldset id=\"" IDPrefix "_" $3 "\" legend=\"" $2 "\">";
    for(i=4;i<=NF;i+=1)
    {
      print "<div class='select_item'><input class=\"form_select\" id=\"" IDPrefix "_" $3 "_" $i "\" type=\"radio\" name=\"" $3 "\" value=\"" $i "\" <!--isSelected($record,'" $3 "','" $i "')-->>" $i "</input></div>";
    }
    print "</fieldset></td>";
    }
/check/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_check\">";
    print "<label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><fieldset id=\"" IDPrefix "_" $3 "\" legend=\"" $2 "\">";
    for(i=4;i<=NF;i+=1)
    {
      print "<div class='check_item'><input class=\"form_check\" type=\"checkbox\" id=\"" IDPrefix "_" $3 "_" $i "\" name=\"" $3 "_" $i "\" <!--isChecked($record,'" $3 "','" $i "')-->>" $i "</input></div>";
    }  
    print "</fieldset></td>";
    }
/int/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_text\"><label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><input id=\"" IDPrefix "_" $3 "\" name=\"" $3 "\" value=\"<!--$record['" $3 "']-->\" maxlength=\"" $6 "\" size=\"" $5 "\"/></td>";
    }
/text/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_text\"><label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><input id=\"" IDPrefix "_" $3 "\" name=\"" $3 "\" value=\"<!--$record['" $3 "']-->\" maxlength=\"" $6 "\" size=\"" $5 "\"/></td>";
    }
/boolean/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_boolean\"><input class=\"form_boolean\" id=\"" IDPrefix "_" $3 "\" type=\"checkbox\" name=\"" $3 "\" <!--boolChecked($record,'" $3 "')-->>" $2 "</input></td>";
    }
/yesno/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_select\"><label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><fieldset id=\"" IDPrefix "_" $3 "\" legend=\"" $2 "\">";
    print "<div class='select_item'><input class=\"form_select\" id=\"" IDPrefix "_" $3 "_yes\" type=\"radio\" name=\"" $3 "\" value=\"yes\" <!--isSelected($record,'" $3 "','yes')-->>yes</input></div>";
    print "<div class='select_item'><input class=\"form_select\" id=\"" IDPrefix "_" $3 "_no\" type=\"radio\" name=\"" $3 "\" value=\"no\" <!--isSelected($record,'" $3 "','no')-->>no</input></div>";
    print "</fieldset></td>\n";
    }
/mmdd/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_text\"><label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><input id=\"" IDPrefix "_" $3 "\" name=\"" $3 "\" value=\"<!--$record['" $3 "']-->\" maxlength=\"5\" size=\"5\"/></td>";
    }
/hhmm/ {
    if (NR != 1 && (NR-1) % 3 == 0) print "</tr><tr>";
    print "<td class=\"form_text\"><label for=\"" IDPrefix "_" $3 "\">" $2 ":</label><input id=\"" IDPrefix "_" $3 "\" name=\"" $3 "\" value=\"<!--$record['" $3 "']-->\" maxlength=\"5\" size=\"5\"/></td>";
    }
