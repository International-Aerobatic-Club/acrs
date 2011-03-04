<?php
/*  include/dbCommand.mssql.inc, acrs/include, dclo, 10/24/2010
 *  low level abstraction interface between ACRS and MS-SQL database
 *
 *  Changes:
 *    10/24/2010 jim_ward       use new DEFINE'd constants, rather than vars.
 */

require_once('dbConfig.inc');
require_once('useful.inc');

// global for last generated identifier
$lastID = 0;
/**
Connect to the database.
Requires database connection handle passed by reference.
Returns failure HTML content, empty if successful.
*/
function dbConnect(&$db_conn)
{
   return dbConnectl(DBPREFIX.DBUSER, DBPASSWD, $db_conn);
}

/**
Connect to the database.
Requires database connection handle passed by reference.
Returns failure HTML content, empty if successful.
*/
function dbConnectl($dbuser, $dbpasswd, &$db_conn)
{
   $fail = '';
   $db_conn = mssql_connect(DBHOST, DBPREFIX.$dbuser, $dbpasswd);
   if (!$db_conn) 
   {
      $fail = notifyError("Failed database connection", DBPREFIX.$dbuser."@".DBHOST);
   }
   else
   {
      $dbsel = DBPREFIX.DBNAME;
      if (!mssql_select_db($dbsel))
      {
         $fail = notifyError("Failed database selection", DBHOST."/".$dbsel);
      }
   }
   return $fail;
}


function mssql_error()
{
   $result = mssql_query('select @@error');
   $row = mssql_fetch_row($result);
   return $row[0];
}

function dbExec($db_conn, $query)
{
   $fail = '';
   $result = mssql_query($query, $db_conn);
   if ($result===false)
   {
      $fail = notifyError(mssql_error(), $query);
   }
   return $fail;
}

function dbQuery($db_conn, $query)
{
   $result = mssql_query($query, $db_conn);
   if ($result === false)
   {
      notifyError(mssql_error(), $query);
   }
   return $result;
}

function dbErrorNumber()
{
   return mssql_error();
}

function dbErrorText()
{
   return mssql_error();
}

function dbCountResult($result)
{
   return mssql_num_rows($result);
}

function dbLastID($db_conn)
{
   $lastID = null;
   $query = 'select @@IDENTITY as insertID';
   $result = mssql_query($query, $db_conn);
   if (!$result)
   {
      $lastID = notifyError(mssql_error(), $query);
   }
   else
   {
      $row = mssql_fetch_row($result);
      $lastID = $row[0];
   }
   return $lastID;
}

function dbFetchAssoc($result)
{
   return mssql_fetch_array($result);
}

function dbFetchRow($result)
{
   return mssql_fetch_row($result);
}

function dbCommit($db_conn)
{
   return mssql_query("commit", $db_conn);
}

function dbClose($db_conn)
{
   if ($db_conn) mssql_close($db_conn);
}

/*
   Copyright 2008, 2010 International Aerobatic Club, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
?>
