<?php
set_include_path('./include'); 
require ("ui/validate.inc");
require ('data/validCtst.inc');
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ("data/encodeSQL.inc");
require_once ("form/regform.inc");
require_once ("query/userQueries.inc");
require_once ("ui/emailNotices.inc");
require_once ("useful.inc");
require_once ("data/timecheck.inc");
require_once ("redirect.inc");

function validateForm(&$registrant)
{
    $corrMsg = '';
    $registrant["givenName"] = crop($registrant["givenName"], 80);
    if (strlen($registrant["givenName"]) == 0)
    {
        $corrMsg .= "<li>Provide a given name.</li>";
    }
    $registrant["familyName"] = crop($registrant["familyName"], 80);
    if (strlen($registrant["familyName"]) == 0)
    {
        $corrMsg .= "<li>Provide a family name.</li>";
    }
    $registrant["email"] = crop($registrant["email"], 80);
    if (strlen($registrant["email"]) == 0)
    {
        $corrMsg .= "<li>Provide an email address.</li>";
    }
    else
        if (!validEmail($registrant["email"]))
        {
            $corrMsg .= "<li>The format of the email address is not valid.</li>";
        }
    $registrant["contactPhone"] = crop($registrant["contactPhone"], 16);
    if (strlen($registrant["contactPhone"]) == 0)
    {
        $corrMsg .= "<li>Provide your phone number.</li>";
    }
    if (boolChecked($registrant, "isStudent"))
    {
        if (strlen(crop($registrant["university"], 48)) == 0)
        {
            $corrMsg .= "<li>Provide your school.</li>";
        }
        if (strlen(crop($registrant["program"], 32)) == 0)
        {
            $corrMsg .= "<li>Provide your program.</li>";
        }
    }
    return $corrMsg;
}

function doRegistration()
{
    $wasUpdated = FALSE;
    $corrMsg = '';
    $userID = $_SESSION['userID'];
    $ctstID = $_SESSION['ctstID'];
    $logEmail = $_SESSION['email'];
    $registrant = $_POST;
    $catList = null;
    $db_conn = false;
    $fail = dbConnect($db_conn);
    if ($fail != '')
    {
        notifyError($fail, "register.php");
        $corrMsg .= "<it>Internal: failed access to contest database</it>";
        $readRecord = false;
    }
    else
    {
        $readRecord = true;
        $fail = getCategoryList($db_conn, $ctstID, $catList);
        if ($fail != '')
        {
            notifyError($fail, "register.php");
            $corrMsg .= "<it>Internal: failed access to category records of " . $ctstID . "</it>";
        }
        if (isset ($_POST["submit"]) || isset ($_POST["save"]))
        {
            //debugArr('register post',$registrant);
            $corrMsg = validateForm($registrant);
            if ($corrMsg == '')
            {
                // have valid data
                $fail = updateRegistration($db_conn, $registrant, $userID);
                if ($fail == '')
                {
                    $wasUpdated = true;
                }
                else
                {
                    notifyError($fail, "register.php");
                    $corrMsg .= "<it>Internal: failed data update.</it>";
                }
            }
            $readRecord = FALSE;
        }
        if ($readRecord)
        {
            // not POST
            $fail = retrieveRegistrant($db_conn, $registrant, $userID);
            if ($fail != '')
            {
                notifyError($fail, "register.php");
                $corrMsg .= "<it>Internal: failed access to registration record of " . $userID . "</it>";
            }
            if (!isset($registrant['catID']) || !isset($catList[$registrant['catID']]))
            {
                $cats = array_keys($catList);
                $registrant['catID'] = $cats[0];
            }
            //debugArr('registrant record ', $registrant);
        }
        dbClose($db_conn);
    }
    if ($wasUpdated)
    {
        // post change interface
        if (isset ($_POST["submit"]))
        {
            $nextURL = "volunteer.php";
        }
        else
        {
            $nextURL = "index.php";
            sendUpdateEmail($logEmail, $registrant);
        }
        getNextPage($nextURL);
    }
    else
    {
        // registration formtype filter text
        // $corrMsg has HTML content
        // $registrant has POST content
        startHead("Registration");
        registrationFormHeader();
        $cat = $catList[$registrant['catID']];
        startContent('onload="setEnabledCompetitor('.
                tfBool($cat, "hasTeamReg") . ',' .
        tfBool($cat, "hasStudentReg").','.
        tfBool($cat, 'hasFourMinute').')"'); 
        echo "<h1>Registration Information</h1>";
        verificationHeader("For,");
        if ($corrMsg != '')
        {
            echo '<ul style="color:red; font-weight:bold">' . $corrMsg . '</ul>';
        }
        registrationForm($registrant, $catList, "register.php");
        echo '<div class="returnButton"><a href="index.php">Return without saving</a></div>';
        endContent();
    }
}

if (isRegOpen())
{
    doRegistration();
}
else
{
    startHead("Registration");
    registrationFormHeader();
    startContent();
    echo '<p class="error">On-line registration is closed.  On-site registration will open before the contest.</p>';
    echo '<div class="returnButton"><a href="index.php">List of registrants</a></div>';
    endContent();
}

/*
   Copyright 2008 International Aerobatic Club, Inc.

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
