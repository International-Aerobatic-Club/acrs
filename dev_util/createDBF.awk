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
/int/ {
    print $3, $4 ",\n";
    }
/text/ {
    print $3, $4 "(" $6 "),\n";
    }
/select/ {
    print $3, "enum(";
    showSet();
    print "),\n";
    }
/check/ {
    print $3, "set(";
    showSet();
    print "),\n";
    }
/boolean/ {
    print $3, "enum('y', 'n') not null default ";
    if ($5 == "true" || $5 == "y")
    {
       print "'y'";
    }
    else
    {
       print "'n'";
    }
    print ",\n";
    }
