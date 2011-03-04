<?php
/*  testDBCommand.php, acrs/admin, dlco, 10/23/2010
 *  database table function tests
 *
 *  Changes:
 *    10/23/2010 jim_ward       include new pilot certificate types.  Use TEST_EMAIL_SERVER
 *                              constant rather than hard-coded value.
 */

require_once ('../acrs/include/dbConfig.inc');
require ('dbSetup.php');

function testEmptyTable($db_conn, $testable)
{
    $pass = false;
    $query = 'select * from ' . $testable;
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        echo '<p>FAIL testEmptyTable ' . $testable . ' query failure.' . '</p>';
    } else
        if (dbCountResult($result) == 0)
        {
            echo '<p>testEmptyTable ' . $testable . ' found table empty.' . '</p>';
            $pass = true;
        } else
        {
            echo '<p>FAIL testEmptyTable ' . $testable . ' nonzero count records.' . '</p>';
        }
    return $pass;
}

function initRecord(& $record, $accountName)
{
    $record['accountName'] = "'" . $accountName . "'";
    $record['password'] = "'password'";
    $record['updated'] = "now()";
    $record['email'] = "'" . $accountName . "@" . TEST_EMAIL_SERVER . "'";
    $record['startDate'] = "now()";
    $record['startTime'] = "now()";
}

function testInsert($db_conn, $testable, $record)
{
    $pass = false;
    $query = 'insert into ' . $testable .
    '(accountName, password, updated, email, startDate, startTime)' .
    ' values ' .
    '(' . $record['accountName'] . ',' .
    $record['password'] . ',' .
    $record['updated'] . ',' .
    $record['email'] . ',' .
    $record['startDate'] . ',' .
    $record['startTime'] . ')';
    $fail = dbExec($db_conn, $query);
    if ($fail != '')
    {
        echo '<p>FAIL testInsert ' . $fail . '</p>';
    } else
    {
        echo '<p>testInsert ' . $testable . ' succeeded.' . '</p>';
        $pass = true;
    }
    return $pass;
}

function testLastID($db_conn, $expected)
{
    $pass = false;
    $lastID = dbLastID();
    if ($lastID == $expected)
    {
        echo "<p>testLastID passed</p>";
        $pass = true;
    } else
    {
        echo '<p>failed last id. expected ' . $expected . ' had ' . $lastID . '</p>';
    }
    return $pass;
}

function testQueryAll($db_conn, $testable, $expectedCt)
{
    $pass = false;
    $result = dbQuery($db_conn, "select * from " . $testable);

    if ($result === false)
    {
        echo "<p>FAIL " . strhtml(dbErrorText()) . "</p>";
    } else
    {
        $count = dbCountResult($result);

        if ($count == $expectedCt)
        {
            $cur = 0;

            while ($row = dbFetchAssoc($result))
            {
                echo '<p><b> Row ' . ++ $cur . '</b>';
                foreach ($row as $key => $value)
                    echo '; ' . $key . '=' . $value;
                echo '</p>';

            }
            if ($count == $cur)
            {
                $pass = true;
                echo '<p>testQueryAll passed</p>';
            }
        } else
        {
            echo '<p>failed testQueryAll expected ' . $expectedCt . ' had ' . $count . '</p>';
        }
    }
    return $pass;
}

function testError($db_conn, $testable)
{
    $pass = false;
    $query = 'select bloodyHell from ' . $testable;
    $result = dbQuery($db_conn, $query);
    if ($result === false)
    {
        echo '<p>testError ' . $testable . ' has error code ' . dbErrorNumber() .
         ' message ' . dbErrorText() . '</p>';
         $pass = true;
    }
    return $pass;
}

echo '<html><head></head><body>';

if (isset ($_POST["submit"]))
{
    $testable = "test_table";
    $fail = dbConnectl($_POST['dbuser'], $_POST['dbpwd'], $db_conn);

    /* check connection */
    if ($fail != '')
    {
        echo "<p>Connect failed: " . $fail . "</p>";
        echo "<p>User:" . $_POST['dbuser'] . "</p>";
        echo "<p>Pwd:" . $_POST['dbpwd'] . "</p>";
    } else
    {
        $schema = "userID int unsigned not null auto_increment primary key," .
        "accountName char(32) unique not null," .
        "password char(40) not null," .
        'index(accountName,password),' .
        "updated datetime not null," .
        "admin enum('y', 'n') not null default 'n'," .
        "email text(320) not null," .
        "givenName varchar(72)," .
        "certType enum('none', 'student', 'private', 'commercial', 'atp', 'sport', 'recreational')" .
        " not null default 'none'," .
        "volunteer set('judge','assistJudge', 'recorder', 'boundary', 'runner', 'deadline', 'timer', 'assistChief')," .
        "chapter smallint unsigned," .
        "startDate date not null," .
        "startTime time not null, " .
        "key(chapter)";
        processTable($db_conn, $testable, $schema, false);

        $pass = testEmptyTable($db_conn, $testable);
        $record = array ();
        initRecord($record, 'test1');
        $pass &= testInsert($db_conn, $testable, $record);
        $pass &= testLastID($db_conn, 1);
        initRecord($record, 'test2');
        $pass &= testInsert($db_conn, $testable, $record);
        $pass &= testLastID($db_conn, 2);
        $pass &= testQueryAll($db_conn, $testable, 2);
        $pass &= testError($db_conn, $testable);

        dropTable($db_conn, $testable);

        /* close connection */
        dbClose($db_conn);

        if ($pass)
        {
            echo 'all tests passed' . '</p>';
        } else
        {
            echo 'FAIL at least one test' . '</p>';
        }
    }
}

doUserPwdForm('testDBCommand.php');
echo '</body></html>';
?>