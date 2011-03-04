<?php
require ('dbSetup.php');

echo '<html><head></head><body>';

$user = '';
$pwd = '';
$query = '';

if (isset ($_POST["submit"]))
{
    $user = $_POST['dbuser'];
    $pwd = $_POST['dbpwd'];
    $query = $_POST['dbquery'];
    $fail = dbConnectl($user, $pwd, $db_conn);

    /* check connection */
    if ($fail != '')
    {
        echo "<p>Connect failed: " . $fail . "</p>";
        echo "<p>User:" . $user . "</p>";
        echo "<p>Pwd:" . $pwd . "</p>";
    }
    else
    {
        $result = dbQuery($db_conn, $query);
        if ($result === false)
        {
            echo '<p>Query error is ' . dbErrorText() . '</p>';
        }
        else
        {
            displayResult($result, 'result');
        }
    }

    /* close connection */
    dbClose($db_conn);
}

echo '<form id="admin" class="regForm" action="runQuery.php" method="post">';
doUserPwdInputs($user, $pwd);
echo '<label for="dbquery">Query:</label>';
echo '<textarea id="dbquery" name="dbquery" rows="8" cols="60">';
echo $query;
echo '</textarea>';
echo '<input class="submit" name="submit" type="submit" value="Submit query"/>';
echo '</form>';
echo '</body></html>';
?>