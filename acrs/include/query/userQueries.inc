<?php
/*  userQueries.inc, acrs/include/query, dclo, 10/24/2010
 *  person-related query functions
 *
 *  Changes:
 *    10/24/2010 jim_ward       add sport and recreational licenses.  Remove explicit pass-by-reference to
 *                              getRegistrationBasicData, as that's specified in the function decl rather
 *                              than in each call.
 */

require_once ("data/encodeSQL.inc");
require_once ("data/encodePOST.inc");
require_once ("data/encodeHTML.inc");
require_once ("dbCommand.inc");

/**
 Functions for reading and writing registrant data with SQL
 */

/**
 Write a new, empty registrant record for the registrant if no
 registration record exists.
 db_conn database connection handle
 iac_id: IAC member number
 userID: return safe integer ID of registrant
 return empty string if user exists, failure message on failure
 */
function checkExistsRegistrant($db_conn, $iac_id, & $userID)
{
   $fail = '';
   $query = "select userID from registrant where iacID = " . strsql($iac_id, 12);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail .= "<it>" . dbErrorText() . "<\it>";
   } else
   if (dbCountResult($result) != 0)
   {
      $row = dbFetchRow($result);
      $userID = $row[0];
   } else
   {
      $fail = "no entry for " . $iac_id;
   }
   return $fail;
}

/**
 Insert new registrant record into the database
 db_conn database connection handle
 email unescaped email address
 givenName unescaped name
 familyName unescaped name
 iac_id IAC member number
 userID return value gives new user identifier if not failure
 returns empty string on success, failure message on failure
 */
function insertNewRegistrantRecord($db_conn, $email, $givenName, $familyName, $iac_id, & $userID)
{
   $fail = '';
   $update = "insert into registrant (email, givenName, familyName, iacID) values ('" .
   addslashes($email) . "','" .
   addslashes($givenName) . "','" .
   addslashes($familyName) . "','" .
   addslashes($iac_id) . "');";
   //debug($update);
   $fail = dbExec($db_conn, $update);
   if ($fail == '')
   {
      $userID = dbLastID();
      $update = "commit";
      $fail = dbExec($db_conn, $update);
   }

   return $fail;
}

function ensureRegistrationRecord($db_conn, $regID)
{
   $fail = '';
   $query = 'select catID from registration where regID = ' . $regID . ';';
   //    debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 0)
   {
      $update = 'insert into registration (regID) values (' . $regID . ');';
      //            debug($update);
      $fail = dbExec($db_conn, $update);
   }
   return $fail;
}

/**
 Write a new, empty registration record for the current contest if no
 current contest registration record exists.  Get the registration ID.
 db_conn database connection handle
 regID: registration ID of registrant, returned value
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 compType: safe SQL string with registration type regrets, volunteer, or competitor
 answer: safe SQL string with registrant's message
 return empty string on success, failure message on failure
 */
function checkExistsCurrentRegistration($db_conn, & $regID, $userID,
$ctstID, $compType, $answer)
{
   $fail = '';
   $query = 'select regID, compType from reg_type where userID = ' . $userID . ' and ctstID = ' . $ctstID . ';';
   //    debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      if (dbCountResult($result) == 0)
      {
         $update = 'insert into reg_type (userID, ctstID, compType, answer) values (' . $userID . ',' . $ctstID . ',' . $compType .  ',' .$answer . ');';
         debug($update);
         $fail = dbExec($db_conn, $update);
         if ($fail == '')
         {
            $regID = dbLastID();
         }
      } else
      {
         $row = dbFetchRow($result);
         $regID = $row[0];
         if ($row[1] != $compType)
         {
            $update = 'update reg_type set ';
            $update .= 'compType = ' . $compType;
            $update .= ', answer = ' . $answer;
            $update .= ' where regID = ' . $regID;
            debug($update);
            $fail = dbExec($db_conn, $update);
         }
      }
      if ($fail == '')
      {
         $fail = ensureRegistrationRecord($db_conn, $regID);
      }
   }
   return $fail;
}

/**
 * Return sql ready string containing competition registration type
 */
function getCompType($registrant)
{
   $_enumSet_compType = array (
        'regrets',
        'competitor',
        'volunteer'
        );
        return enumSQL($registrant['compType'], $_enumSet_compType);
}

/**
 Update registrant record in the registrant table
 db_conn database connection handle
 registrant: registrant data in an associative array
 userID: safe integer ID of registrant
 returns empty string on success, failure message on failure
 */
function updateRegistration($db_conn, $registrant, $userID)
{
   //debugArr("Registration post data: ",$registrant);

   // start transaction
   $fail = dbBegin($db_conn);
   $ctstID = $_SESSION['ctstID'];

   // update registrant
   if ($fail == '')
   {
      $update = "update registrant set ";
      $update .= 'email = ' . strSQL($registrant['email'], 320) . ',';
      $update .= 'givenName = ' . strSQL($registrant['givenName'], 72) . ',';
      $update .= 'familyName = ' . strSQL($registrant['familyName'], 72) . ',';
      $update .= 'contactPhone = ' . strSQL($registrant['contactPhone'], 16) . ',';
      $update .= 'address = ' . strSQL($registrant['address'], 72) . ',';
      $update .= 'city = ' . strSQL($registrant['city'], 24) . ',';
      $update .= 'state = ' . strSQL($registrant['state'], 24) . ',';
      $update .= 'country = ' . strSQL($registrant['country'], 24) . ',';
      $update .= 'postalCode = ' . strSQL($registrant['postalCode'], 12) . ',';
      $update .= 'eaaID = ' . strSQL($registrant['eaaID'], 12) . ',';
      $update .= 'iacID = ' . strSQL($registrant['iacID'], 12) . ',';
      $update .= 'faiID = ' . strSQL($registrant['faiID'], 12) . ',';
      $_enumSet_shirtsize = array (
            'XS',
            'S',
            'M',
            'L',
            'XL',
            'XXL'
            );
            $update .= 'shirtsize = ' . enumSQL($registrant['shirtsize'], $_enumSet_shirtsize);
            $update .= ','.'iceName = ' . strSQL($registrant['iceName'], 72) . ',';
            $update .= 'icePhone1 = ' . strSQL($registrant['icePhone1'], 16) . ',';
            $update .= 'icePhone2 = ' . strSQL($registrant['icePhone2'], 16);
            $update .= " where userID = " . $userID . ";";

            //        debug($update);
            $fail = dbExec($db_conn, $update);
   }

   $compType = getCompType($registrant);
   $answer = strSQL($registrant['answer'], 4096);
   $regID = 0;

   // update registration record, create if does not exist
   if ($fail == '')
   {
      $fail = checkExistsCurrentRegistration($db_conn, $regID, $userID,
         $ctstID, $compType, $answer);
   }
   if ($fail == '' && strcmp($compType, "'competitor'") == 0)
   {
      $update = "update registration set ";
      $update .= 'catID = ' . intSQL($registrant['catID']) . ',';
      $update .= 'teamAspirant= ' . boolSQL($registrant, 'teamAspirant') . ',';
      $update .= 'fourMinFree= ' . boolSQL($registrant, 'fourMinFree') . ',';
      $update .= 'ownerName = ' . strSQL($registrant['ownerName'], 72) . ',';
      $update .= 'ownerPhone = ' . strSQL($registrant['ownerPhone'], 16) . ',';
      $update .= 'ownerAddress = ' . strSQL($registrant['ownerAddress'], 72) . ',';
      $update .= 'ownerCity = ' . strSQL($registrant['ownerCity'], 24) . ',';
      $update .= 'ownerCountry = ' . strSQL($registrant['ownerCountry'], 24) . ',';
      $update .= 'ownerState = ' . strSQL($registrant['ownerState'], 24) . ',';
      $update .= 'ownerPostal = ' . strSQL($registrant['ownerPostal'], 12) . ',';
      $update .= 'isStudent= ' . boolSQL($registrant, 'isStudent') . ',';
      $update .= 'university = ' . strSQL($registrant['university'], 48) . ',';
      $update .= 'program = ' . strSQL($registrant['program'], 32) . ',';
      $update .= 'isFirstTime= ' . boolSQL($registrant, 'isFirstTime');
      $update .= ' where regID = ' . $regID . ';';
      //        debug($update);
      $fail = dbExec($db_conn, $update);
   }
   if ($fail == '')
   {
      $fail = dbCommit($db_conn);
   } else
   {
      dbRollback($db_conn);
   }

   return $fail;
}

/**
 Update flying information record in the registrant
 and registration tables
 db_conn database connection handle
 registrant: registrant data in an associative array
 userID: safe integer ID of registrant
 returns empty string on success, failure message on failure
 */
function updateFlying($db_conn, $registrant, $userID)
{
   //debugArr('Flying post data', $registrant);

   $fail = dbBegin($db_conn);
   $ctstID = $_SESSION['ctstID'];

   // update registrant
   if ($fail == '')
   {
      $update = "update registrant set ";
      $_enumSet_certType = array (
            'student',
            'private',
            'commercial',
            'atp',
	    'sport',
	    'recreational'
            );
            $update .= 'certType = ' . enumSQL($registrant['certType'], $_enumSet_certType) . ',';
            $update .= 'certNumber = ' . strSQL($registrant['certNumber'], 16);
            $update .= " where userID = " . $userID . ";";

            //debug($update);
            $fail = dbExec($db_conn, $update);
   }

   $compType = getCompType($registrant);
   $answer = strSQL($registrant['answer'],4096);
   $regID = 0;

   // update registration record, create if does not exist
   if ($fail == '')
   {
      $fail = checkExistsCurrentRegistration($db_conn, $regID, $userID,
         $ctstID, $compType, $answer);
   }
   if ($fail == '')
   {
      $update = "update registration set ";
      $update .= 'currMedical= ' . boolSQL($registrant, 'currMedical') . ',';
      $update .= 'currBiAnn= ' . boolSQL($registrant, 'currBiAnn') . ',';
      $update .= 'currPacked= ' . boolSQL($registrant, 'currPacked') . ',';
      $update .= 'safety = ' . strSQL($registrant['safety'], 72) . ',';
      $update .= 'chapter = ' . strSQL($registrant['chapter'], 6) . ',';
      $update .= 'ownerPilot= ' . boolSQL($registrant, 'ownerPilot') . ',';
      $update .= 'ownerName = ' . strSQL($registrant['ownerName'], 72) . ',';
      $update .= 'ownerPhone = ' . strSQL($registrant['ownerPhone'], 16) . ',';
      $update .= 'ownerAddress = ' . strSQL($registrant['ownerAddress'], 72) . ',';
      $update .= 'ownerCity = ' . strSQL($registrant['ownerCity'], 24) . ',';
      $update .= 'ownerCountry = ' . strSQL($registrant['ownerCountry'], 24) . ',';
      $update .= 'ownerState = ' . strSQL($registrant['ownerState'], 24) . ',';
      $update .= 'ownerPostal = ' . strSQL($registrant['ownerPostal'], 12) . ',';
      $update .= 'airplaneMake = ' . strSQL($registrant['airplaneMake'], 24) . ',';
      $update .= 'airplaneModel = ' . strSQL($registrant['airplaneModel'], 24) . ',';
      $update .= 'airplaneRegID = ' . strSQL($registrant['airplaneRegID'], 16) . ',';
      $update .= 'airplaneColors = ' . strSQL($registrant['airplaneColors'], 24) . ',';
      $_enumSet_airworthiness = array (
            'experimental',
            'acrobatic'
            );
            $update .= 'airworthiness = ' . enumSQL($registrant['airworthiness'], $_enumSet_airworthiness) . ',';
            $update .= 'engineMake = ' . strSQL($registrant['engineMake'], 24) . ',';
            $update .= 'engineModel = ' . strSQL($registrant['engineModel'], 24) . ',';
            $update .= 'engineHP = ' . strSQL($registrant['engineHP'], 6) . ',';
            $update .= 'currInspection = ' . boolSQL($registrant, 'currInspection') . ',';
            $update .= 'insCompany = ' . strSQL($registrant['insCompany'], 24) . ',';
            $update .= 'liabilityAmt= ' . boolSQL($registrant, 'liabilityAmt') . ',';
            $update .= 'injuryAmt= ' . boolSQL($registrant, 'injuryAmt') . ',';
            $update .= 'insExpires = ' . strSQL($registrant['insExpires'], 10);
            $update .= ' where regID = ' . $regID . ';';

            //debug($update);
            $fail = dbExec($db_conn, $update);
   }
   if ($fail == '')
   {
      $fail = dbCommit($db_conn);
   } else
   {
      dbRollback($db_conn);
   }

   return $fail;
}

/**
 Retrieve latest year registered into $ctstID, zero if never registered.
 db_conn database connection handle
 userID: safe integer ID of registrant
 ctstID: reference to latest year
 return empty string on success, failure message on failure
 */
function getLatestReg($db_conn, $userID, & $ctstID)
{
   $ctstID = 0;
   $fail = '';
   $query = 'select ctstID from reg_type'.
   ' where regID in ('.
   ' select max(regID) from reg_type'.
   ' where userID = ' . $userID . 
   " and compType = 'competitor');";
   debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchRow($result);
      $ctstID = $row[0];
   }
   //debug('getLatestReg Contest:'.$ctstID.', fail:'.$fail);
   return $fail;
}

function retrieveRegForUserContest($db_conn, $userID, $ctstID, &$registrant, &$haveReg)
{
   $fail = '';
   $haveReg = false;
   $query = 'select * from registration a, reg_type b' .
        ' where b.userID=' . $userID .
        ' and b.ctstID = ' . $ctstID .
        ' and a.regID = b.regID' .
        ';';
   //debug('userQueries.retrieveRegForUserContest:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) != 1)
   {
      $fail = "no registration for contest id ".$ctstID.", userID ".$userID;
   } else
   {
      $haveReg = true;
      $row = dbFetchAssoc($result);
      foreach ($row as $key => $value)
      {
         $registrant[$key] = stripslashes($value);
      }

      sqlBoolValueToPostData($row['currMedical'], 'currMedical', $registrant);
      sqlBoolValueToPostData($row['currBiAnn'], 'currBiAnn', $registrant);
      sqlBoolValueToPostData($row['currPacked'], 'currPacked', $registrant);
      sqlBoolValueToPostData($row['ownerPilot'], 'ownerPilot', $registrant);
      sqlBoolValueToPostData($row['currInspection'], 'currInspection', $registrant);
      sqlBoolValueToPostData($row['liabilityAmt'], 'liabilityAmt', $registrant);
      sqlBoolValueToPostData($row['injuryAmt'], 'injuryAmt', $registrant);
      sqlBoolValueToPostData($row['needAssist'], 'needAssist', $registrant);
      sqlBoolValueToPostData($row['teamAspirant'], 'teamAspirant', $registrant);
      sqlBoolValueToPostData($row['fourMinFree'], 'fourMinFree', $registrant);
      sqlBoolValueToPostData($row['isStudent'], 'isStudent', $registrant);
      sqlBoolValueToPostData($row['isFirstTime'], 'isFirstTime', $registrant);
      sqlBoolValueToPostData($row['hasVotedJudge'], 'hasVotedJudge', $registrant);
      sqlCheckNullDefault($row['shirtsize'], 'shirtsize', $registrant, 'L');
   }
   return $fail;
}

/**
 Retrieve registration record from the registrant and registration tables
 db_conn database connection handle
 registrant: reference to registrant data in an associative array
 userID: safe integer ID of registrant
 Initializes a record in the registration table with $userID and ctstID = $_SESSION['ctstID']
 if said record does not yet exist.
 returns empty string on success, failure message on failure
 */
function retrieveRegistrant($db_conn, & $registrant, $userID)
{
   $fail = '';
   $ctstID = $_SESSION['ctstID'];

   // retrieve registrant data (unique to user id)
   $query = 'select * from registrant where userID=' . $userID . ';';
   //debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   }
   else
   {
      if (dbCountResult($result) != 1)
      {
         $fail = "registrant ID ".$userID." is not valid";
      } else
      {
         $row = dbFetchAssoc($result);
         foreach ($row as $key => $value)
         {
            $registrant[$key] = stripslashes($value);
         }
      }
   }

   $haveReg = false;
   if ($fail == '')
   {
      // retrieve registration data (unique to user id, contest id)
      $fail = retrieveRegForUserContest($db_conn, $userID, $ctstID, $registrant, $haveReg);
      if (!$haveReg)
      {
         // retrieve prior contest data if present
         $ctstID = 0;
         $fail = getLatestReg($db_conn, $userID, $ctstID);
         if ($fail == '' && 0 < $ctstID)
         {
            $fail = retrieveRegForUserContest($db_conn, $userID, $ctstID, $registrant, $haveReg);
         }
      }
   }
   if (!$haveReg)
   {
      // set some reasonable defaults for the radio buttons
      $registrant['compType'] = 'regrets';
      $registrant['certType'] = 'private';
      $registrant['judgeQualification'] = 'none';
      $registrant['shirtsize'] = 'L';
   }
   //debugArr('retrieveRegistration', $registrant);

   return $fail;
}

function initRegistrationBasicData(& $registrant)
{
   $registrant['paidAmt'] = 0;
   $registrant['airplaneRegID'] = '';
   $registrant['insCompany'] = '';
   $registrant['compType'] = 'regrets';
   sqlBoolValueToPostData(false, 'isRegistered', $registrant);
   sqlBoolValueToPostData(false, 'teamAspirant', $registrant);
   sqlBoolValueToPostData(false, 'fourMinFree', $registrant);
   sqlBoolValueToPostData(false, 'isStudent', $registrant);
   sqlBoolValueToPostData(false, 'hasVotedJudge', $registrant);
   sqlBoolValueToPostData(false, 'hasStudentReg', $registrant);
   sqlBoolValueToPostData(false, 'hasTeamReg', $registrant);
   sqlBoolValueToPostData(false, 'hasVoteJudge', $registrant);
   sqlBoolValueToPostData(false, 'voteTeamOnly', $registrant);
   sqlBoolValueToPostData(false, 'voteByRegion', $registrant);
   sqlBoolValueToPostData(false, 'ctstHasVoteJudge', $registrant);
   sqlBoolValueToPostData(false, 'reqPmtForVoteJudge', $registrant);
   sqlBoolValueToPostData(false, 'hasPayPal', $registrant);
   sqlBoolValueToPostData(false, 'hasPracticeReg', $registrant);
   sqlBoolValueToPostData(true, 'reqPmtForPracticeReg', $registrant);
   sqlBoolValueToPostData(false, 'catHasVoteJudge', $registrant);
}

function getRegistrationBasicData($db_conn, $userID, $ctstID, & $registrant)
{
   $fail = '';
   $needInit = true;
   $query = 'select regID, compType';
   $query .= ' from reg_type where';
   $query .= ' userID = ' . $userID;
   $query .= ' and ctstID = ' . $ctstID;
   //debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      if (dbCountResult($result) == 1)
      {
         $registrant['isRegistered'] = true;
         $row = dbFetchAssoc($result);
         foreach ($row as $key => $value)
         {
            $registrant[$key] = stripslashes($value);
         }
         $query = 'select  ' .
            'b.name, b.catID, b.class, b.category, b.regAmt, b.hasStudentReg, b.studentRegAmt, ' .
            'b.hasTeamReg, b.teamRegAmt, b.hasVoteJudge, b.maxVotes, ' .
            'b.voteTeamOnly, b.voteByRegion, b.maxRegion, b.voteDeadline, ' .
            'b.hasFourMinute, b.fourMinRegAmt, ' .
            'a.teamAspirant, a.fourMinFree, a.airplaneMake, a.airplaneRegID, a.insCompany, ' .
            'a.isStudent, a.paidAmt, a.hasVotedJudge ';
         $query .= ' from registration a, ctst_cat b, ' .
            'reg_type c where';
         $query .= ' c.userID = ' . $userID;
         $query .= ' and c.ctstID = ' . $ctstID;
         $query .= ' and a.regID = c.regID';
         $query .= ' and b.catID = a.catID';
         //debug($query);
         $result = dbQuery($db_conn, $query);
         if ($result === false)
         {
            $fail = dbErrorText();
         } else
         {
            //debug("have " . dbCountResult($result) . " registration entries.");
            if (dbCountResult($result) == 1)
            {
               $row = dbFetchAssoc($result);
               foreach ($row as $key => $value)
               {
                  $registrant[$key] = stripslashes($value);
               }
               sqlBoolValueToPostData($row['hasFourMinute'], 'hasFourMinute', $registrant);
               sqlBoolValueToPostData($row['hasStudentReg'], 'hasStudentReg', $registrant);
               sqlBoolValueToPostData($row['hasTeamReg'], 'hasTeamReg', $registrant);
               sqlBoolValueToPostData($row['hasVoteJudge'], 'hasVoteJudge', $registrant);
               sqlBoolValueToPostData($row['voteTeamOnly'], 'voteTeamOnly', $registrant);
               sqlBoolValueToPostData($row['voteByRegion'], 'voteByRegion', $registrant);
               sqlBoolValueToPostData($row['teamAspirant'], 'teamAspirant', $registrant);
               sqlBoolValueToPostData($row['fourMinFree'], 'fourMinFree', $registrant);
               sqlBoolValueToPostData($row['isStudent'], 'isStudent', $registrant);
               sqlBoolValueToPostData($row['hasVotedJudge'], 'hasVotedJudge', $registrant);
               $needInit = false;
            }
         }
      }
   }
   if ($needInit)
   {
      initRegistrationBasicData($registrant);
   }
   return $fail;
}

/**
 Retrieve basic registration status information for the given year
 and registrant id
 db_conn database connection handle
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 registrant: reference to registrant data in an associative array
 registrant will contain:
 givenName, familyName, iacID, email, isRegistered
 compType, compClass, compCat, teamAspirant,
 fourMinFree, isStudent, paidAmt, hasVotedJudge,
 airplaneRegID, insCompany
 returns registrant data in POST data format.
 returns empty string on success, failure message on failure
 */
function getRegistrantBasicData($db_conn, $userID, $ctstID, & $registrant)
{
   $fail = '';
   $needInit = true;

   // retrieve registrant data (unique to user id)
   $query = 'select givenName, familyName, email, iacID from registrant';
   $query .= ' where userID=' . $userID;
   //    debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchAssoc($result);
      foreach ($row as $key => $value)
      {
         $registrant[$key] = stripslashes($value);
      }
      if (isset ($ctstID))
      {
         $fail = getRegistrationBasicData($db_conn, $userID, $ctstID, $registrant);
      }
      $needInit = $fail != '';
   }
   if ($needInit)
   {
      $registrant['givenName'] = '';
      $registrant['familyName'] = '';
      $registrant['email'] = '';
      $registrant['iacID'] = '';
      initRegistrationBasicData($registrant);
   }
   return $fail;
}

function extractCategoryRow($row, & $category)
{
   foreach ($row as $key => $value)
   {
      $category[$key] = $value;
   }
   sqlBoolValueToPostData($row['hasStudentReg'], 'hasStudentReg', $category);
   sqlBoolValueToPostData($row['hasTeamReg'], 'hasTeamReg', $category);
   sqlBoolValueToPostData($row['hasVoteJudge'], 'hasVoteJudge', $category);
   sqlBoolValueToPostData($row['voteTeamOnly'], 'voteTeamOnly', $category);
   sqlBoolValueToPostData($row['voteByRegion'], 'voteByRegion', $category);
   sqlBoolValueToPostData($row['hasFourMinute'], 'hasFourMinute', $category);
   // debugArr("userQueries.extractCategoryRow:", $row);
}

/**
 * Create array $catList and populate it with contest categories.
 */
function getCategoryList($db_conn, $ctstID, & $catList)
{
   $fail = '';
   $query = 'select * ';
   $query .= ' from ctst_cat where';
   $query .= ' ctstID=' . $ctstID;
   $query .= ' order by catID';
   //debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   {
      $catList = array ();
      while ($row = dbFetchAssoc($result))
      {
         $cat = array ();
         extractCategoryRow($row, $cat);
         $catList[$cat['catID']] = $cat;
      }
   }
   return $fail;
}

/**
 * Populate $catData with category data for contest, class, category
 */
function getCategoryData($db_conn, $catID, & $catData)
{
   $fail = '';
   $query = 'select * ';
   $query .= ' from ctst_cat where';
   $query .= ' catID=' . $catID;
   //debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchAssoc($result);
      extractCategoryRow($row, $catData);
   } else
   {
      $fail = "no record for category id " . $catID;
   }
   return $fail;
}

/**
 Compute the fee due for the registrant.
 returns registration fee amount
 */
function computeRegistrationFee($reg)
{
   $fee = 0;
   if (isSelected($reg, "compType", "competitor"))
   {
      //      debugArr('userQueries.computeRegistrationFee:', $reg);
      $fee = $reg['regAmt'];
      if (boolChecked($reg, 'teamAspirant') && boolChecked($reg, 'hasTeamReg'))
      {
         $fee = $reg['teamRegAmt'];
      }
      else if (boolChecked($reg, 'isStudent') && boolChecked($reg, 'hasStudentReg'))
      {
         $fee = $reg['studentRegAmt'];
      }
      if (boolChecked($reg, 'fourMinFree') && boolChecked($reg, 'hasFourMinute'))
      {
         $fee += $reg['fourMinRegAmt'];
      }
   }
   return $fee;
}

/**
 Determine whether the registrant has fully paid registration.
 reg contains at least:
 isStudent, compType, compClass, compCat, teamAspirant, fourMinFree
 "regAmt smallint unsigned," .
 "hasStudentReg enum('y', 'n') not null default 'n'," .
 "studentRegAmt smallint unsigned," .
 "hasTeamReg enum('y', 'n') not null default 'n'," .
 "teamRegAmt smallint unsigned," .
 returns true if the registrant has paid the full fee or more.
 */
function checkPaidInFull($reg)
{
   $fee = computeRegistrationFee($reg);
   return $fee <= $reg['paidAmt'];
}

/**
 * Determine whether to display and accept practice registration.
 * Requires current contest in $_SESSION['contest'] (see setcontest.php)
 * reg contains at least:
 * isStudent,
 * compType,
 * compClass,
 * compCat,
 * teamAspirant,
 * fourMinFree
 *       "regAmt smallint unsigned," .
 *       "hasStudentReg enum('y', 'n') not null default 'n'," .
 *       "studentRegAmt smallint unsigned," .
 *       "hasTeamReg enum('y', 'n') not null default 'n'," .
 *       "teamRegAmt smallint unsigned," .
 * Return true if practice registration enabled.
 */
function havePracticeRegistration($contest, $reg)
{
   //debugArr("\nhavePracticeReg contest:", $contest);
   //debugArr("\nhavePracticeReg reginfo:", $reg);
   $enable = false;
   if (sqlIsTrue($contest['hasPracticeReg']))
   {
      if (sqlIsTrue($contest['reqPmtForPracticeReg']))
      {
         $enable = checkPaidInFull($reg);
      } else
      {
         $enable = true;
      }
   }
   return $enable;
}

/**
 * Determine whether to display and accept judge ballots.
 * Requires current contest in $_SESSION['contest'] (see setcontest.php)
 * reg contains at least:
 * isStudent,
 * compType,
 * compClass,
 * compCat,
 * teamAspirant,
 * fourMinFree
 *       "regAmt smallint unsigned," .
 *       "hasStudentReg enum('y', 'n') not null default 'n'," .
 *       "studentRegAmt smallint unsigned," .
 *       "hasTeamReg enum('y', 'n') not null default 'n'," .
 *       "teamRegAmt smallint unsigned," .
 *       "hasVoteJudge enum('y', 'n') not null default 'n'," .
 *       "maxVotes smallint unsigned," .
 *       "voteTeamOnly enum('y', 'n') not null default 'n'," .
 * Return true if practice registration enabled.
 */
function haveJudgeVoting($contest, $reg)
{
   //debugArr('haveJudgeVoting', $reg);
   $enable = false;
   if (sqlIsTrue($contest['hasVoteJudge']) && boolChecked($reg, 'hasVoteJudge'))
   {
      if (strcmp($contest['reqPmtForVoteJudge'], 'y') == 0)
      {
         $enable = checkPaidInFull($reg);
      } else
      {
         $enable = true;
      }
   }
   return $enable;
}

/**
 Retrieve contest information for the given contest
 db_conn database connection handle
 ctstID: safe integer ID of contest
 contest: reference to registrant data in an associative array
 contest will contain:
 givenName, familyName, iacID, email, isRegistered
 compType, compClass, compCat, teamAspirant,
 fourMinFree, isStudent, paidAmt, hasVotedJudge,
 airplaneRegID, insCompany
 returns contest data in POST data format.
 returns empty string on success, failure message on failure
 */
function getContestData($db_conn, $ctstID, & $contest)
{
   $fail = '';

   // retrieve contest data (unique to contest id)
   $query = 'select * from contest';
   $query .= ' where ctstID=' . $ctstID;
   //    debug($query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchAssoc($result);
      foreach ($row as $key => $value)
      {
         $contest[$key] = stripslashes($value);
      }
      sqlBoolValueToPostData($row['hasVoteJudge'], 'hasVoteJudge', $contest);
      sqlBoolValueToPostData($row['reqPmtForVoteJudge'], 'reqPmtForVoteJudge', $contest);
      sqlBoolValueToPostData($row['hasPayPal'], 'hasPayPal', $contest);
      sqlBoolValueToPostData($row['hasPracticeReg'], 'hasPracticeReg', $contest);
      sqlBoolValueToPostData($row['reqPmtForPracticeReg'], 'reqPmtForPracticeReg', $contest);
   }
   else {
      $fail = 'Contest id "'.$ctstID.'" not available.';
   }
   return $fail;
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
