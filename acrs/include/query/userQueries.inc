<?php
/*  userQueries.inc, acrs/include/query, dclo, 10/24/2010
 *  person-related query functions
 *
 *  Changes:
 *    10/24/2010 jim_ward    add sport and recreational licenses.
 *      Remove explicit pass-by-reference to getRegistrationBasicData, as that's
 *      specified in the function decl rather than in each call.
 *    07/20/2011 dclo        rewrite queries so registration data
 *      carries from one registration to the next
 */

require_once ("data/encodeSQL.inc");
require_once ("data/encodePOST.inc");
require_once ("data/encodeHTML.inc");
require_once ("query/catQueries.inc");
require_once ("dbCommand.inc");

/**
 Functions for reading and writing registrant data with SQL
 */

/**
 Read registration record for given regID.
 db_conn database connection handle
 regID: registration ID of registrant
 regdata: associative array updated by this call
 return empty string on success, failure message on failure
*/
function getRegType($db_conn, $regID, & $registrant)
{
   //debug("here getRegType");
   $fail = '';
   $query = "select * from reg_type where regID = $regID;";
   //debug('getRegType:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchAssoc($result);
      //debugArr('getRegType has row:', $row);
      foreach ($row as $key => $value)
      {
         $registrant[$key] = stripslashes($value);
      }
   }
   //debug("leaving getRegType says, $fail ...");
   return $fail;
}

/**
 Create a new registration record.
 db_conn database connection handle
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 regID: registration ID of registrant, returned value
 compType: SQL safe string with comp type 'regrets', 'competitor', or
 'volunteer'
 return empty string on success, failure message on failure
*/
function createRegType($db_conn, $userID, $ctstID, & $regID, $compType)
{
   $regID = 0;
   $update = 'insert into reg_type (userID, ctstID, compType, answer) values (';
   $update .= $userID . ',' . $ctstID.','.strSQL($compType,32).",'');";
   //debug('createRegType:'.$update);
   $fail = dbExec($db_conn, $update);
   if ($fail == '')
   {
      $regID = dbLastID();
   }
   //debug("createRegType new regID is $regID, $fail");
}

/**
 Update the reg_type record for user and contest
 db_conn database connection handle
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 regID: registration ID of registrant, returned value
 registrant: registration data
 return empty string on success, failure message on failure
 */
function updateRegType($db_conn, $userID, $ctstID, & $regID, $registrant)
{
   $fail = getContestReg($db_conn, $userID, $ctstID, $regID);
   if ($fail == '' && $regID != 0)
   {
     $_enumSet_compType = array (
        'regrets',
        'competitor',
        'volunteer'
      );
      $update = 'update reg_type set ';
      $update .= 'compType = ' . enumSQL($registrant['compType'], $_enumSet_compType);
      $update .= ', answer = ' . strSQL($registrant['answer'],4096);
      $update .= ' where regID = ' . $regID;
      //debug('updateRegType:'.$update);
      $fail = dbExec($db_conn, $update);
   }
   return $fail;
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
      //debug('updateRegistration:'.$update);
      $fail = dbExec($db_conn, $update);
   }

   $regID = 0;
   if ($fail == '')
   {
      $fail = updateRegType($db_conn, $userID, $ctstID, $regID, $registrant);
   }

   if ($fail == '' && $regID != 0)
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
      //debug('updateRegistration:'.$update);
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

      //debug('updateFlying:'.$update);
      $fail = dbExec($db_conn, $update);
   }

   $regID = 0;
   if ($fail == '')
   {
     $fail = updateRegType($db_conn, $userID, $ctstID, $regID, $registrant);
   }

   if ($fail == '' && $regID != 0)
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

      //debug('updateFlying:'.$update);
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
 Retrieve registration for user and contest, zero if never registered.
 db_conn database connection handle
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 regID: return id of this contest registration
 return empty string on success, failure message on failure
 */
function getContestReg($db_conn, $userID, $ctstID, & $regID)
{
   $regID = 0;
   $fail = '';
   $query = 'select regID from reg_type where userID = ' . $userID . 
     ' and ctstID = ' . $ctstID . ';';
   //debug('getContestReg:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchRow($result);
      $regID = $row[0];
   }
   //debug('getContestReg using regID:'.$regID.', '.$fail);
   return $fail;
}

/**
 Retrieve most recent registration for user, zero if never registered.
 db_conn database connection handle
 userID: safe integer ID of registrant
 regID: return id of most recent prior registration as competitor
 return empty string on success, failure message on failure
 */
function getLatestReg($db_conn, $userID, & $regID)
{
   $regID = 0;
   $fail = '';
   $query = 'select max(regID) from reg_type where userID = ' . $userID . 
     " and compType = 'competitor';";
   //debug('getLatestReg:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchRow($result);
      $regID = $row[0];
   }
   // debug("getLatestReg using regID:$regID, says $fail ...");
   return $fail;
}

/**
 Initialize default registration record for user and contest
 db_conn: database connection handle
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 regID: return id of this contest registration
 return empty string on success, failure message on failure
 */
function initRegDefault($db_conn, $userID, $ctstID, & $regID)
{
  // debug("here initRegDefault");
  $catID = 0;
  $fail = createRegType($db_conn, $userID, $ctstID, $regID, 'competitor');
  if ($fail == '')
  {
     $fail = findProperRegCat($db_conn, $ctstID, 'power', 'primary', $catID);
  }
  if ($fail == '')
  {
    $insert = 'insert into registration (regID, catID) values ('
    . $regID . ',' . $catID . ');';
    // debug("initRegDefault: $insert");
    $fail = dbExec($db_conn, $insert);
  }
  // debug("leaving initRegDefault says, $fail ...");
  return $fail;
}

function tryQueryCatID($db_conn, $ctstID, & $catID, $limit)
{
   $fail = '';
   $query = 'select max(catID) from ctst_cat where ctstID = ' . $ctstID;
   if (0 < strlen(trim($limit)))
   {
     $query .= ' and ' . $limit;
   }
   $query .= ';';
   //debug('tryQueryCatID:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchRow($result);
      $catID = $row[0];
   }
   //debug("tryQueryCatID has catID: $catID, $fail");
   return $fail;
}

/**
 For the given contest, find the category most nearly matching
   className, catName
 db_conn: database connection
 ctstID: id of contest
 className: string 'power' or 'glider'
 catName: string 'primary', 'sportsman', etc.
 catID: returned id of matching category in contest
*/
function findProperRegCat($db_conn, $ctstID, $catClass, $catCategory, & $catID)
{
  // matching category and class
  $className = enumSQL($catClass, array('power', 'glider', 'other'));
  $catName = enumSQL($catCategory, array('primary','sportsman',
     'intermediate','advanced','unlimited','4min','other'));
  $fail = tryQueryCatID($db_conn, $ctstID, $catID,
    "class = $className and category = $catName");
  if ($fail == '' && $catID == 0)
  {
    // matching class
    $fail = tryQueryCatID($db_conn, $ctstID, $catID, "class = $className");
  }
  if ($fail == '' && $catID == 0)
  {
    // matching category
    $fail = tryQueryCatID($db_conn, $ctstID, $catID, "category = $catName");
  }
  if ($fail == '' && $catID == 0)
  {
    // any category
    $fail = tryQueryCatID($db_conn, $ctstID, $catID, '');
  }
  if ($fail == '' && $catID == 0)
  {
    // this only happens if there are no categories for the contest
    $name = ucwords($catClass . ' ' . $catCategory);
    $insert = 'insert into ctst_cat (ctstID, name, class, category) values ('
    . $ctstID
    .  ',' . strSQL($name, 72)
    .  ',' . $className
    . ',' . $catName
    . ');';
    //debug("findProperRegCat: $insert");
    $fail = dbExec($db_conn, $insert);
    if ($fail == '')
    {
      $catID = dbLastID();
    }
  }
  //debug("findProperRegCat using category $catID");
  return $fail;
}

/**
 Initialize registration record for user and contest using data from
   prior contest
 db_conn: database connection handle
 userID: safe integer ID of registrant
 ctstID: safe integer ID of contest
 latestRegID: safe integer ID of prior registration
 regID: return id of this contest registration
 return empty string on success, failure message on failure
 */
function initRegFromPrior($db_conn, $userID, $ctstID, $latestRegID, & $regID)
{
   // debug("Here initRegFromPrior");
   $priorReg = array();
   $catID = 0;
   $fail = doRetrieveRegistration($db_conn, $latestRegID, $priorReg);
   if ($fail == '')
   {
     //debugArr('initRegFromPrior', $priorReg);
     $fail = findProperRegCat($db_conn, $ctstID, $priorReg['class'],
       $priorReg['category'], $catID);
   }
   if ($fail == '')
   {
     $fail = createRegType($db_conn, $userID, $ctstID, $regID, 'competitor');
   }
   if ($fail == '')
   {
      $_enumSet_airworthiness = array (
            'experimental',
            'acrobatic'
            );
      $update = 'insert into registration (regID, catID'
      . ', safety, chapter, ownerPilot, ownerName, ownerPhone'
      . ', ownerAddress, ownerCity, ownerCountry, ownerState, ownerPostal'
      . ', airplaneMake, airplaneModel, airplaneRegID, airplaneColors'
      . ', airworthiness, engineMake, engineModel, engineHP'
      . ', insCompany, insExpires'
      . ') values ('
      . $regID . ',' . $catID
      . ',' . strSQL($priorReg['safety'], 72) 
      . ',' . strSQL($priorReg['chapter'], 6)
      . ',' . boolSQL($priorReg, 'ownerPilot') 
      . ',' . strSQL($priorReg['ownerName'], 72) 
      . ',' . strSQL($priorReg['ownerPhone'], 16) 
      . ',' . strSQL($priorReg['ownerAddress'], 72) 
      . ',' . strSQL($priorReg['ownerCity'], 24) 
      . ',' . strSQL($priorReg['ownerCountry'], 24) 
      . ',' . strSQL($priorReg['ownerState'], 24) 
      . ',' . strSQL($priorReg['ownerPostal'], 12) 
      . ',' . strSQL($priorReg['airplaneMake'], 24) 
      . ',' . strSQL($priorReg['airplaneModel'], 24) 
      . ',' . strSQL($priorReg['airplaneRegID'], 16) 
      . ',' . strSQL($priorReg['airplaneColors'], 24) 
      . ',' . enumSQL($priorReg['airworthiness'], $_enumSet_airworthiness) 
      . ',' . strSQL($priorReg['engineMake'], 24) 
      . ',' . strSQL($priorReg['engineModel'], 24) 
      . ',' . strSQL($priorReg['engineHP'], 6) 
      . ',' . strSQL($priorReg['insCompany'], 24) 
      . ',' . strSQL($priorReg['insExpires'], 10)
      . ');';
      //debug('initRegFromPrior:'.$update);
      $fail = dbExec($db_conn, $update);
   }
   // debug("Leaving initRegFromPrior says, $fail ...");
   return $fail;
}

/**
  Find the regID.
  Find the existing registration.
  Initialize a new registration record if it does not exist.
  Initialize from a prior registration if possible.
  Populate $registrant with reg_type data.
*/
function doRetrieveRegType($db_conn, $userID, $ctstID, & $regID, & $registrant)
{
   // debug("here doRetrieveRegType");
   $fail = getContestReg($db_conn, $userID, $ctstID, $regID);
   if ($fail == '' && $regID == 0)
   {
      $latestRegID = 0;
      $fail = getLatestReg($db_conn, $userID, $latestRegID);
      if ($fail == '')
      {
         if ($latestRegID == 0)
         {
            $fail = initRegDefault($db_conn, $userID, $ctstID, $regID);
         }
         else
         {
            $fail = initRegFromPrior($db_conn, $userID, $ctstID,
              $latestRegID, $regID);
         }
      }
   }
   if ($fail == '')
   {
      $fail = getRegType($db_conn, $regID, $registrant);
   }
   return $fail;
}

// retrieve registration data (unique to regID)
function doRetrieveRegistration($db_conn, $regID, & $registrant)
{
   // debug("Here doRetrieveRegistration");
   $fail = '';
   $query = 'select a.*, b.class, b.category from registration a, ctst_cat b'
     . ' where a.regID = ' . $regID . ' and b.catID = a.catID;';
   //debug('doRetrieveRegistration:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
   if (dbCountResult($result) == 1)
   {
      $row = dbFetchAssoc($result);
      // debug("doRetrieveRegistration found one record for id $regID");
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
   }
   // debug("Leaving doRetrieveRegistration says, $fail ...");
   return $fail;
}

// retrieve registrant data (unique to userID)
function doRetrieveRegistrant($db_conn, $userID, & $registrant)
{
   $fail = '';
   $query = 'select * from registrant where userID=' . $userID . ';';
   //debug('doRetrieveRegistrant:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
      $fail = dbErrorText();
   } else
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
      sqlCheckNullDefault($row['certType'], 'certType', $registrant, 'none');
   }
   return $fail;
}

/**
 Retrieve registration record from the registrant and registration tables
 If registration does not exist, initialize based on
 most recent registration, if any, lowest competitor category if not.
 db_conn: database connection handle
 userID: safe integer ID of registrant
 ctstID: safe intereg ID of contest
 registrant: reference to registrant data in an associative array
 returns empty string on success, failure message on failure
 */
function retrieveRegistrant($db_conn, $userID, $ctstID, & $registrant)
{
   //debug('retrieveRegistrant...');
   $fail = doRetrieveRegistrant($db_conn, $userID, $registrant);

   $regID = 0;
   if ($fail == '')
   {
     // enusures registration initialized and returns regID
     $fail = doRetrieveRegType($db_conn, $userID, $ctstID, $regID, $registrant);
   }

   if ($fail == '')
   {
      $fail = doRetrieveRegistration($db_conn, $regID, $registrant);
   }
   //debugArr("retrieveRegistrant has:", $registrant);
   //debug("...retrieveRegistrant $fail");

   return $fail;
}

function initRegistrationBasicData(& $registrant)
{
   $registrant['paidAmt'] = 0;
   $registrant['airplaneRegID'] = ''; 
   $registrant['insCompany'] = ''; 
   $registrant['compType'] = 'regrets';
   $registrant['airworthiness'] = 'experimental';
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

/**
  Retrieve registration information.
  Retrieve only what exists.  Do not create any new data.
  Initialize defaults for what does not exist.
  Used by the index page to detect what menu items to show.
*/
function retrieveExistingRegData($db_conn, $userID, $ctstID, & $registrant)
{
  //debug('retrieveExistingRegData...');
  $fail = doRetrieveRegistrant($db_conn, $userID, $registrant);
  $regID = 0;
  if ($fail == '')
  {
    $fail = getContestReg($db_conn, $userID, $ctstID, $regID);
  }
  if ($fail == '')
  {
    if ($regID == 0)
    {
      initRegistrationBasicData($registrant);
    }
    else
    {
      $fail = getRegType($db_conn, $regID, $registrant);
      if ($fail == '')
      {
        $fail = doRetrieveRegistration($db_conn, $regID, $registrant);
      }
      if ($fail == '')
      {
        $catID = $registrant['catID'];
        if (!isset($catID) || $catID == 0)
        {
          // legacy recovery
          $fail = findProperRegCat($db_conn, $ctstID, 'power', 'primary', $catID);
        }
        if ($fail == '')
        {
          $fail = retrieveCategory($db_conn, $registrant, $catID);
        }
      }
    }
  }
  //debugArr("retrieveExistingRegData has:", $registrant);
  //debug("...retrieveExistingRegData $fail");
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
 Copyright 2008, 2010, 2011 International Aerobatic Club, Inc.

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
