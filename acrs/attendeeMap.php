<?php
/*  attendeeMap.php, acrs/admin, dlco, 10/20/2012
 *  View map of contest registrants by zip code
 */
set_include_path('./include'); 
require ("ui/validate.inc");
require ("data/validCtst.inc");
require_once ('post/setContest.inc');
require_once('dbConfig.inc');
require_once ("dbCommand.inc");
require_once ('data/timecheck.inc');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("query/userQueries.inc");

function generate_marker_data($db_conn, $ctstID)
{
   $query = 'select a.postalCode, ' .
    ' e.category, e.name, e.hasTeamReg, b.teamAspirant ' .
    ' from registrant a, registration b, ctst_cat e, reg_type f' .
    ' where a.userID = f.userID ' .
    ' and f.ctstID = ' . $ctstID .
    " and f.compType = 'competitor'" .
    ' and b.regID = f.regID' .
    ' and e.catID = b.catID' .
    ' order by a.postalCode';
    //debug('generate_marker_data:'.$query);
   $result = dbQuery($db_conn, $query);
   if ($result === false)
   {
     echo '<p class="error">' .
       notifyError(dbErrorText(),'generate_marker_data') . '</p>';
   }
   else
   {
     if (0 < dbCountResult($result))
     {
       echo 'var records = [';
       $first = true;
       while ($curRcd = dbFetchAssoc($result))
       {
         if (!$first) echo ",\n";
         $first = false;
         $isTeam = $curRcd['hasTeamReg'] == 'y' && 
           $curRcd['teamAspirant'] == 'y' ?
             'true' : 'false';
         echo "{zip:'" . $curRcd['postalCode'] .  
           "',name:'" . $curRcd['name'] . 
           "',cat:'" . $curRcd['category'] . 
           "',team:'" . $isTeam . 
           "'}";
       }
       echo "];\n";
     }
   }
}

$fail = dbConnect($db_conn);
$ctstID = $_SESSION['ctstID'];
startHead("Contest Participant Map");
if ($fail == '') {
?>
<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
<script type="text/javascript">
    <?php generate_marker_data($db_conn, $ctstID) ?>
</script>
<script type="text/javascript" src="contestMap.js"></script>
<?php
}
startContent();
echo '<h1>Contest Participant Map</h1>';
echo '<a href="index.php">Back</a>';
echo '<div id="map_canvas"></div>';
endContent();
?>
