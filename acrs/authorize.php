<?php
/*  authorize.php, acrs, jim_ward, 03/03/11
 *  "authorize contest officials" form
 *
 *  Changes:
 *      03/03/2011 jim_ward     limit the list of possible contest officials to those individuals who are registered to
 *                              volunteer or fly in this contest.
 */

set_include_path('./include');
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeHTML.inc");
require_once ("data/encodeSQL.inc");
require_once ("data/encodePOST.inc");
require_once ("ui/siteLayout.inc");
require_once ("useful.inc");

$_enumSet = array (
    'admin',
    'cd',
    'registrar',
    'vc'
);

/*
 * Create a key from user ID and user role
 * userID - registrant key
 * role - 'cd', 'registrar', or 'vc' (roles column of ctst_admin table)
 * return parseable, html safe key composed of userID, role
 */
function makeKey($userID, $role)
{
   return $userID . '_' . $role;
}

/*
 * Parse practice slot date and index from data key.
 * key input slot identifier
 * sessID return session ID of slot
 * slotIdx return index of slot
 */
function parseKey($key, & $userID, & $role)
{
   $i1 = strpos($key, '_');
   $rem = '';
   if ($i1 > 0)
   {
      $userID = substr($key, 0, $i1);
      $role = substr($key, ($i1 +1));
   }
   //debug('parseKey: key=' . $key . ', i1=' . $i1 . ', userID=' . $userID . ', role=' . $role);
}

/**
 Write one permission checkbox form entry.
 */
function writeRole($userID, $role)
{
   $key = makeKey($userID, $role);
   echo '<td class="authorize-check">' .
    '<input class="form_check" type="checkbox" id="' .
   $key . '" name="role:' . $key . '"/>';
   echo "</td>\n";
}

function writeCurrentRole($userID, $role)
{
   $key = makeKey($userID, $role);
   echo '<td class="authorize-check">' .
    '<input class="form_check" type="checkbox" id="' .
   $key . '" name="role:' . $key;
   echo '" checked="true"/>';
   echo '<input type="hidden" name="preselect:' . $key . '" value="on"/>';
   echo "</td>\n";
}

function doWriteRole($roles, $userID, $role)
{
   if (testChecked($roles, 'roles_' . $userID, $role))
   {
      writeCurrentRole($userID, $role);
   } else
   {
      writeRole($userID, $role);
   }
}

function getExistingRoles($db_conn, & $roleSets)
{
   global $_enumSet;
   $query = 'select userID, roles from ctst_admin' .
    ' where ctstID = ' . $_SESSION['ctstID'];
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      notifyError(dbErrorText(), "authorize.php");
      $corrMsg .= '<it>Internal: failed user query</it>';
   } else
   {
      while ($row = dbFetchAssoc($result))
      {
         sqlSetValueToPostData($row['roles'], 'roles_' . $row['userID'], $roleSets);
      }
   }
}

function showForm($db_conn, $roleSets)
{
   echo '<div style="text-align:center">';

   // Display only those people who are either administrators or registrants for the current contest
   $query = 'SELECT registrant.userID, accountName, givenName, familyName ' .
            'FROM registrant ' .
            'LEFT JOIN ctst_admin ON registrant.userID=ctst_admin.userID ' .
            'LEFT JOIN reg_type ON registrant.userID=reg_type.userID ' .
            'WHERE ctst_admin.ctstID=' . intSQL($_SESSION['ctstID']) . ' OR reg_type.ctstID=' . intSQL($_SESSION['ctstID']) . ' ' .
            'GROUP BY userID ' .
            'ORDER BY familyName, givenName' ;
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      notifyError(dbErrorText(), "authorize.php");
      $corrMsg .= '<it>Internal: failed user query</it>';
   } else
   {
      $columnCount = 1;
      echo '<form method="POST" action="authorize.php">';
      echo '<table class="authorize">' .
        '<thead class="authorize"><tr class="authorize">';
      for ($i = 0; $i < $columnCount; ++ $i)
      {
         echo '<th>Name</th><th>CD</th><th>Reg</th><th>VC</th>';
         //echo '<th>Admin</th>';
      }
      echo '</tr></thead><tbody class="authorize"><tr class="authorize">' . "\n";
      $row = dbFetchAssoc($result);
      $i = 0;
      while ($row)
      {
         echo '<td class="authorize-name">' . strhtml($row["givenName"]) . ' ' .
         strhtml($row["familyName"]) . ' (' .
         strhtml($row['accountName']) . ')</td>';
         $userID = $row['userID'];
         //debug('authorize.php roles for ' . $userID . ' are ' . $roles);
         doWriteRole($roleSets, $userID, 'cd');
         doWriteRole($roleSets, $userID, 'registrar');
         doWriteRole($roleSets, $userID, 'vc');
         //doWriteRole($roleSets, $userID, 'admin');
         if (testChecked($roleSets, 'roles_' . $userID, 'admin'))
         {
            $admin = makeKey($userID, 'admin');
            echo '<input type="hidden" name="preselect:' . $admin . '" value="on"/>';
            echo '<input type="hidden" name="role:' . $admin . '" value="on"/>';
         }
         if ($i % $columnCount == $columnCount -1)
         echo '</tr><tr class="authorize">';
         $i += 1;
         $row = dbFetchAssoc($result);
      }
      echo '</tr></tbody></table>' . "\n";
      echo '<input class="submit" name="submit" type="submit" value="Update Roles"/>' . "\n";
      echo '</form>';
   }
   echo '</div>';
   echo '<h5>Note: Only those people who are registered to volunteer or fly in this contest appear in this list.</h5>' ;
}

function updatePermissions($db_conn, $permissions)
{
   global $_enumSet;
   $corrMsg = '';
   //debugArr('authorize.php submit', $permissions);
   $toSet = array ();
   $hadSet = array ();
   foreach ($permissions as $key => $value)
   {
      if (strncmp($key, 'role:', 5) == 0)
      {
         $key = substr($key, 5);
         $userID = null;
         $role = null;
         parseKey($key, $userID, $role);
         //debug('authorize.php has user ' . $userID . ' now in role ' . $role);
         $toSet[$userID] = true;
      }
      if (strncmp($key, 'preselect:', 10) == 0)
      {
         $key = substr($key, 10);
         $userID = null;
         $role = null;
         parseKey($key, $userID, $role);
         //debug('authorize.php has user ' . $userID . ' was in role ' . $role);
         $hadSet[$userID] = true;
      }
   }
   forEach ($toSet as $userID => $value)
   {
      $hadSet[$userID] = false;
      $roles = selectionSQL($permissions, "role:" . $userID, $_enumSet);
      $corrMsg .= insertOrUpdatePermissions($db_conn, $userID, $roles);
   }
   forEach ($hadSet as $userID => $toClear)
   {
      if ($toClear)
      {
         $corrMsg .= removePermissions($db_conn, $userID);
      }
   }
   return $corrMsg;
}

function roleExists($db_conn, $userID)
{
   $haveRole = false;
   $query = "select roles from ctst_admin where userID = " . $userID . " and ctstID = " . intSQL($_SESSION['ctstID']);
   //debug('authorize.roleExists() query:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      notifyError(dbErrorText(), 'roleExists');
   } else
   {
      //debug('authorize.roleExists():count = '.dbCountResult($result));
      $haveRole = (dbCountResult($result) != 0);
      //        if ($haveRole)
      //        {
      //            while($row = dbFetchAssoc($result))
      //            {
      //                //debugArr('authorize has existing',$row);
      //            }
      //        }
      }
      return $haveRole;
   }

   function insertOrUpdatePermissions($db_conn, $userID, $roles)
   {
      $corrMsg = '';
      if (roleExists($db_conn, $userID))
      {
         $query = "update ctst_admin set " .
         $query .= ' roles = ' . $roles . ' ';
         $query .= ' where userId = ' . $userID;
         $query .= " and ctstID = " . intSQL($_SESSION['ctstID']);
      } else
      {
         $query = "insert into ctst_admin(userID, ctstID, roles) values";
         $query .= '(' . $userID . "," . intSQL($_SESSION['ctstID']) . ",";
         $query .= $roles . ")";
      }
      //debug('authorize.php:' . $query);
      $corrMsg = dbExec($db_conn, $query);
      if ($corrMsg != '')
      $corrMsg = '<li>' . $corrMsg . '</li>';
      return $corrMsg;
   }

   function removePermissions($db_conn, $userID)
   {
      $corrMsg = '';
      $query = 'delete from ctst_admin' .
    ' where userID = ' . $userID .
    ' and ctstID = ' . intSQL($_SESSION['ctstID']);
      //debug('authorize.php:' . $query);
      $corrMsg = dbExec($db_conn, $query);
      if ($corrMsg != '')
      $corrMsg = '<li>' . $corrMsg . '</li>';
      return $corrMsg;
   }

   function doAuthorize($db_conn)
   {
      $corrMsg = '';
      if (isset ($_POST["submit"]))
      {
         $corrMsg = updatePermissions($db_conn, $_POST);
         if ($corrMsg == '')
         {
            echo '<p>Roles updated.</p>';
            echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
         }
      }
      if ($corrMsg == '')
      {
         $roleSets = array ();
         $corrMsg = getExistingRoles($db_conn, $roleSets);
         //debugArr('authorize.php existing roles', $roleSets);
         if ($corrMsg == '')
         showForm($db_conn, $roleSets);
      }
      return $corrMsg;
   }

   $corrMsg = '';
   $fail = dbConnect($db_conn);
   if ($fail != '')
   {
      notifyError($fail, 'authorize.php');
      $corrMsg = '<it>Internal: failed access to member database</it>' . "\n";
   }
   startHead("Authorize Contest Officials");
   echo '<link href="regform.css" type="text/css" rel="stylesheet"/>';
   echo '<link href="print.css" type="text/css" rel="stylesheet"/>';
   startContent();
   echo '<h1 class="noprint">Authorize Officials</h1>';
   if ($corrMsg != '')
   {
      echo '<ul class="error">' . $corrMsg . '</ul>';
   } else
   {
      if (isContestAdmin())
      {
         $corrMsg = doAuthorize($db_conn);
      } else
      {
         $corrMsg = '<li>Restricted to contest officials.</li>';
      }
   }
   if ($db_conn)
   dbClose($db_conn);
   if ($corrMsg != '')
   echo '<p class="error"><ul class="error">' . $corrMsg . '</ul></p>';
   echo '<p class="noprint"><a href="index.php">Return to registration</a></p>';
   endContent();
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
