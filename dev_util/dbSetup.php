<?php
/*  dbSetup.php, admin, dclo, 10/24/2010
 *  database setup utility functions
 *
 *  Changes:
 *    10/24/2010 jim_ward       update include file ref.
 */

require_once ('../acrs/include/dbCommand.inc');

function displayResult($result, $rowTitle='')
{
    $count = dbCountResult($result);
    $foundData = 0 < $count;
    for ($cur = 1; $cur <= $count; ++ $cur)
    {
        $row = dbFetchAssoc($result);
        echo '<p><b>'.$rowTitle.' Row ' . $cur . '</b>';
        foreach ($row as $key => $value)
            echo '; ' . $key . '=' . $value;
        echo '</p>';
    }
    return $foundData;
}

function checkTable($db_conn, $table)
{
    $result = dbQuery($db_conn, "select * from " . $table);

    if (dbErrorNumber() != 0)
    {
        echo "<p>" . strhtml(dbErrorText()) . "</p>";
    }
    else
        if (!displayResult($result, $table))
        {
            echo "<p>" . $table . " table is empty</p>";
        }
}

function dropTable($db_conn, $table)
{
    $query = "drop table " . $table;
    $result = dbExec($db_conn, $query);
    if ($result == '')
    {
        echo "<p>dropped " . $table . "</p>";
    }
    else
    {
        echo "<p>error " . $result . " on drop " . $table . " table</p>";
    }
}

function runQueryShowResult($db_conn, $query)
{
    $fail = '';
    $result = dbQuery($db_conn, $query);
    if (dbErrorNumber() != 0)
    {
        $fail = dbErrorText();
    }
    else
    {
        if (!displayResult($result, $query))
        {
            $fail = "query, '" . $query . "' returned an empty result.";
        }
    }
    return $fail;
}

function processTable($db_conn, $tableName, $schema, $reset = false)
{
    if ($reset)
        dropTable($db_conn, $tableName);

    $query = 'create table if not exists ' . $tableName . '(' . $schema . ')';
    $result = dbExec($db_conn, $query);
    if ($result == '')
    {
        echo '<p>The ' . $tableName . ' table exists.</p>';
    }
    else
    {
        echo '<p>error ' . $result . ' on create table, "' . $tableName . '"</p>';
    }

    checkTable($db_conn, $tableName);
}

function alterTable($db_conn, $tableName, $schema)
{
    $query = 'alter table ' . $tableName . ' ' . $schema . ';';
    $result = dbExec($db_conn, $query);
    if ($result == '')
    {
        echo '<p>Alteration to ' . $tableName . ' succeeded.</p>';
        checkTable($db_conn, $tableName);
    }
    else
    {
        echo '<p>error ' . $result . ' on alter table, "' . $tableName . '"</p>';
    }
}

function doUserPwdInputs($user = '', $pwd = '')
{
    echo '<label for="dbuser">Database user:</label>';
    echo '<input id="dbuser" type="text" name="dbuser" value="'.$user.'" maxlength="16" size="16"/>';
    echo '<label for="dbpwd">User password:</label>';
    echo '<input id="dbpwd" type="password" name="dbpwd" value="'.$pwd.'" maxlength="32" size="32"/>';
}

function doUserPwdForm($action, $user = '', $pwd = '')
{
    echo '<form id="admin" class="regForm" action="' . $action . '" method="post">';
    doUserPwdInputs($user, $pwd);
    echo '<input class="submit" name="submit" type="submit" value="Initialize database"/>';
    echo '</form>';
}
?>
