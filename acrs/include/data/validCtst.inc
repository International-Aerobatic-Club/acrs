<?php
/*  validCtst.inc, acrs/include/data, dclo, 10/24/2010
 *  contest ID and and role functions
 *
 *  Changes:
 *    10/24/2010 jim_ward       sense isset() of $_SESSION['ctst_admin'] and friends before referencing
 *                              them, to suppress PHP notices.
 */

require_once ('post/setContest.inc');

// assumes valid user, i.e. $_SESSION['userID'] is set
// include ui/validate.inc before this file
// they are separate files because some pages do not need a current contest,
// for example, the contest selection page and the contest add page
if (isset ($_GET['ctstID']) && $_GET['ctstID'])
{
    $fail = setContest($_GET['ctstID']);
    if ($fail != '')
    {
        echo '<html><body>';
        echo '<p>' . notifyError($fail, 'data/validCtst.inc') . '</p>';
        echo '</body></html>';
        exit ();
    }
}
if (!isset ($_SESSION['ctstID']))
{
    // get the contest from the user
    header("Location: selectCtst.php?url=" . urlencode($_SERVER["SCRIPT_NAME"]));
    exit ();
}

/**
 * Return true if logged-in user is enabled to edit the contest information,
 * including categories and practice sessions.  The admin and the cd
 * have this authorization.
 */
function isContestAdmin()
{
    return ((isset ($_SESSION['ctst_admin']) && $_SESSION['ctst_admin']) ||
            (isset ($_SESSION['ctst_cd'])    && $_SESSION['ctst_cd']));
}

/**
 * Return true if logged-in user is enabled to view registration reports.  
 * The admin, cd, and registrar have this authorization.
 */
function isRegistrar()
{
    return (isContestAdmin() ||
            (isset ($_SESSION['ctst_registrar']) && $_SESSION['ctst_registrar']));
}

/**
 * Return true if logged-in user is enabled to view some contest admin functions.
 */
function isContestOfficial()
{
    //debugArr('validCtst session variables:',$_SESSION);
    //debug('validCtst.isContestOfficial vc:'.$_SESSION['ctst_vc'].', registrar:'.$_SESSION['ctst_registrar']);
    return (isRegistrar() || (isset ($_SESSION['ctst_vc']) && $_SESSION['ctst_vc']));
}
?>
<?php
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
