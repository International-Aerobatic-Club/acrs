<?php
/*  initDB.php, acrs/admin, dlco, 10/23/2010
 *  create tables in acrs database
 *
 *  Changes:
 *    10/23/2010 jim_ward	include new pilot certificate types.
 */

require ('dbSetup.php');

echo '<html><head></head><body>';

if (isset ($_POST["submit"]))
{
    $fail = dbConnectl($_POST['dbuser'], $_POST['dbpwd'], $db_conn);

    /* check connection */
    if ($fail != '')
    {
        echo "<p>Connect failed: " . $fail . "</p>";
        echo "<p>User:" . $_POST['dbuser'] . "</p>";
        echo "<p>Pwd:" . $_POST['dbpwd'] . "</p>";
    }
    else
    {
        /*
         * REGISTRANT
         * identity basic information for one account
         * userID PRIMARY key
         *          */
        $schema = "userID int unsigned not null auto_increment primary key," .
        "accountName char(32) unique not null," .
        "password char(40) not null," .
        'index(accountName,password),' .
        "updated datetime not null," .
        "admin enum('y', 'n') not null default 'n',".
        "email text(320) not null," .
        "givenName varchar(72)," .
        "familyName varchar(72)," .
        "contactPhone char(16)," .
        "address varchar(72)," .
        "city varchar(24)," .
        "state varchar(24)," .
        "country varchar(24)," .
        "postalCode char(12)," .
        "certType enum('none', 'student', 'private', 'commercial', 'atp', 'sport', 'recreational')" .
        " not null default 'none'," .
        "certNumber char(16)," .
        "eaaID char(12)," .
        "iacID char(12)," .
        "faiID char(12)," .
        "judgeQualification enum('none', 'regional', 'national')" .
        "not null default 'none'," .
        "shirtsize enum('XS', 'S', 'M', 'L', 'XL', 'XXL') not null default 'L',".
        "iceName varchar(72),".
        "icePhone1 char(16),".
        "icePhone2 char(16)";
        processTable($db_conn, 'registrant', $schema, false);

        /*
         * REG_TYPE
         * ties one participant to one contest
         * regID PRIMARY key unique to userID, ctstID
         * userID foreign to REGISTRANT
         * ctstID foreign to CONTEST
         */
        $schema = "regID int unsigned not null auto_increment primary key,".
        " userID int unsigned not null," .
        " ctstID int unsigned not null," .
        " compType enum('regrets', 'competitor', 'volunteer')" .
        " not null default 'competitor'," .
        " unique (userID, ctstID)";
        processTable($db_conn, 'reg_type', $schema, false);

        /*
         * REGISTRATION
         * one participant's registration data for one contest
         * regID foreign to REG_TYPE gives registrant and contest
         * catID foreign to CTST_CAT gives category description and contest
         */
        $schema = "regID int unsigned not null primary key," .
        "catID int unsigned not null," .
        "chapter char(6)," .
        "teamAspirant enum('y', 'n') not null default 'n'," .
        "fourMinFree enum('y', 'n') not null default 'n'," .
        "currMedical enum('y', 'n') not null default 'n'," .
        "currBiAnn enum('y', 'n') not null default 'n'," .
        "currPacked enum('y', 'n') not null default 'n'," .
        "safety varchar(72)," .
        "ownerPilot enum('y', 'n') not null default 'y'," .
        "ownerName varchar(72)," .
        "ownerPhone char(16)," .
        "ownerAddress varchar(72)," .
        "ownerCity varchar(24)," .
        "ownerCountry varchar(24)," .
        "ownerState varchar(24)," .
        "ownerPostal char(12)," .
        "airplaneMake varchar(24)," .
        "airplaneModel varchar(24)," .
        "airplaneRegID char(16)," .
        "airplaneColors varchar(24)," .
        "airworthiness enum('experimental', 'acrobatic')" .
        " not null default 'experimental'," .
        "engineMake varchar(24)," .
        "engineModel varchar(24)," .
        "engineHP char(6)," .
        "currInspection enum('y', 'n') not null default 'n'," .
        "insCompany varchar(24)," .
        "liabilityAmt enum('y', 'n') not null default 'n'," .
        "injuryAmt enum('y', 'n') not null default 'n'," .
        "insExpires char(10)," .
        "isStudent enum('y', 'n') not null default 'n'," .
        "university varchar(48)," .
        "program varchar(32)," .
        "isFirstTime enum('y', 'n') not null default 'n'," .
        "paidAmt int," .
        "hasVotedJudge enum('y', 'n') not null default 'n'";
        processTable($db_conn, 'registration', $schema, false);

        /*
         * VOLUNTEER
         * one participants volunteer data for one contest category
         * userID foreign to REGISTRANT
         * catID foreign to CTST_CAT
         */
        $schema = "userID int unsigned not null," .
        "catID int unsigned not null," .
        "volunteer set('judge','assistJudge', 'recorder', 'boundary', 'runner', 'deadline', 'timer', 'assistChief')," .
        "unique(userID, catID)";
        processTable($db_conn, "volunteer", $schema, false);

        /*
         * JUDGE
         * record judge eligible for contest voting
         * ctstID foreign to CONTEST
         */
        $schema =
        "ctstID int unsigned not null," .
        "givenName varchar(72)," .
        "familyName varchar(72)," .
        "contactPhone char(16)," .
        "iacID char(12) not null," .
        "region enum('northeast', 'southeast', 'midamerica', 'southcentral', 'northwest', 'southwest')," .
        "availableDate date," .
        "voteCount smallint not null default 0," .
        "unique(iacID, ctstID)";
        processTable($db_conn, "judge", $schema, false);

        /*
         * PPTXN
         * record paypal transaction
         * regID foreign to REGISTRATION
         */
        $schema = "txn_id char(17) not null," .
        "regID int unsigned not null," . // custom
        "pay_date char(28) not null," .
        "item_name char(127) not null," .
        "pay_amt decimal(6,2) not null," . // mc_gross
        "currency char(3) not null," . // mc_currency
        "payer_email varchar(127) not null," .
        "first_name varchar(64) not null," .
        "last_name varchar (64) not null," .
        "unique(txn_id)," .
        "key(regID)";
        processTable($db_conn, "pptxn", $schema, false);

        /*
         * CONTEST
         * record contest basic information
         * ctstID PRIMARY key
         */
        $schema = "ctstID int unsigned not null auto_increment primary key," .
        "regYear smallint unsigned not null," .
        "name varchar(72) not null," .
        "location varchar(72)," .
        "chapter smallint unsigned," .
        "startDate date not null," .
        "endDate date not null," .
        "regOpen date not null," .
        "regDeadline date not null," .
        "homeURL text(320) not null," .
        "regEmail text(320) not null," .
        "hasVoteJudge enum('y', 'n') not null default 'n'," .
        "reqPmtForVoteJudge enum('y', 'n') not null default 'n'," .
        "voteEmail text(320)," .
        "hasPayPal enum('y', 'n') not null default 'n'," .
        "payEmail text(320)," .
        "hasPracticeReg enum('y', 'n') not null default 'n'," .
        "reqPmtForPracticeReg enum('y', 'n') not null default 'n'," .
        "maxPracticeSlots smallint unsigned";
        processTable($db_conn, "contest", $schema, false);

        /*
         * CTST_ADMIN
         * record administrative access to contest functions
         * ctstID foreign to CONTEST
         * userID foreign to REGISTRANT
         */
        $schema = "ctstID int unsigned not null," .
        "userID int unsigned not null," .
        "roles set('admin', 'cd', 'registrar', 'vc')," .
        "unique(ctstID, userID)";
        processTable($db_conn, "ctst_admin", $schema, false);

        /*
         * CTST_CAT
         * record category information for one category of one contest
         * catID PRIMARY key
         * ctstID foreign to CONTEST
         */
        $schema = "catID int unsigned not null auto_increment primary key," .
        "ctstID int unsigned not null," .
        "name varchar(72)," .
        "class enum('power', 'glider', 'other') not null," .
        "category enum('primary', 'sportsman', 'intermediate', 'advanced', 'unlimited', '4min', 'other')" .
        "not null," .
        "regAmt smallint unsigned," .
        "hasStudentReg enum('y', 'n') not null default 'n'," .
        "studentRegAmt smallint unsigned," .
        "hasTeamReg enum('y', 'n') not null default 'n'," .
        "teamRegAmt smallint unsigned," .
        "hasVoteJudge enum('y', 'n') not null default 'n'," .
        "maxVotes smallint unsigned," .
        "voteTeamOnly enum('y', 'n') not null default 'n'," .
        "voteByRegion enum('y', 'n') not null default 'n'," .
        "maxRegion smallint unsigned," .
        "voteDeadline date," .
        "hasFourMinute enum('y', 'n') not null default 'n'," .
        "fourMinRegAmt smallint unsigned," .
        "key(ctstID)";
        processTable($db_conn, "ctst_cat", $schema, false);

        /*
         * SESSION
         * this is an available practice session, containing practice slots
         * sessID PRIMARY key
         * ctstID foreign to CONTEST
         */
        $schema = "sessID int unsigned not null auto_increment primary key, " .
        "ctstID int unsigned not null, " .
        "practiceDate date not null, " .
        "startTime time not null, " .
        "endTime time not null, " .
        "minutesPer smallint unsigned not null, " .
        "maxSlotsPer smallint unsigned," .
        "key(ctstID)";
        processTable($db_conn, "session", $schema, false);

        /*
         * PRACTICE_SLOT
         * this is a practice slot reservation
         * sessID foreign to SESSION
         * userID foreign to REGISTRANT
         */
        $schema = 
        "sessID int unsigned not null," .
        "slotIndex smallint not null," .
        "userID int unsigned not null," .
        "unique(sessID, slotIndex)," .
        "key(sessID)";
        processTable($db_conn, "practice_slot", $schema, false);

        /*
         * SLOT_RESTRICTION
         * this is a practice slot restriction
         * sessID foreign to SESSION
         * catID foreign to CTST_CAT
         */
        $schema =
        "sessID int unsigned not null," .
        "slotIndex smallint not null," .
        "restrictionType enum('class', 'category') default 'class'," .
        "class enum('power', 'glider', 'other') default 'other'," .
        "catID int unsigned," .
        "unique(sessID, slotIndex)," .
        "key(sessID)";
        processTable($db_conn, "slot_restriction", $schema, false);
        
        /* close connection */
        dbClose($db_conn);
    }
}

doUserPwdForm('initDB.php');
echo '</body></html>';
?>