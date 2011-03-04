<?php
set_include_path('../acrs/include');
require("dbCommand.inc");

function checkTable($db_conn, $table)
{
   $result = dbQuery($db_conn, "select * from ".$table);

   if (dbErrorNumber() != 0)
   {
      echo "<p>".strhtml(dbErrorText())."</p>";
   }
   else if (dbCountResult($result) != 0)
   {
      echo "<p>".$table." table has data"."</p>";
      $row = dbFetchAssoc($result);
      while ($row)
      {
         echo "<table border='2px' style='display:inline;'><tbody>";
         foreach ($row as $key => $value) echo "<tr><td>".$key."</td><td>".$value."</tr>";
         echo "</tbody></table>";
         $row = dbFetchAssoc($result);
      }
   }
   else
   {
      echo "<p>".$table." table is empty</p>";
   }
}
?>

<html><head></head><body>

<?php
if (isset($_POST["submit"]))
{
   $fail = dbConnectl($_POST['dbuser'], $_POST['dbpwd'], $db_conn);

   /* check connection */
   if ($fail != '') {
      echo "<p>Connect failed: ".$fail."</p>";
      echo "<p>User:".$_POST['dbuser']."</p>";
      echo "<p>Pwd:".$_POST['dbpwd']."</p>";
   }
   else
   {
      $query = "ALTER TABLE registrant ADD ".
        "iceName varchar(72), add ".
        "icePhone1 char(16), add ".
        "icePhone2 char(16)";
      
      $result = dbExec($db_conn,$query);
      if ($result == '')
      {
         echo "<p>registrant updated</p>";
      }
      else
      {
         echo "<p>error ".$result." updating table 'registrant.'</p>";
      }
      
      checkTable($db_conn, "registrant");
      
      /* close connection */
      dbClose($db_conn);
   }
}

?>

<form id="admin" class="regForm" action="updateDB.php" method="post">
<label for="dbuser">Database user:</label>
<input id="dbuser" type="text" name="dbuser" value="admin" maxlength="16" size="16"/>
<label for="dbpwd">User password:</label>
<input id="dbpwd" type="password" name="dbpwd" value="" maxlength="32" size="32"/>
<input class="submit" name="submit" type="submit" value="Initialize database"/>
</form>
</body></html>