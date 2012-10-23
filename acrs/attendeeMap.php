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
    ' e.name, e.hasTeamReg, b.teamAspirant ' .
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
       echo '<script type="text/javascript">';
       echo 'records = [';
       $first = true;
       while ($curRcd = dbFetchAssoc($result))
       {
         if (!$first) echo ',';
         $first = false;
         echo "{zip:'" . $curRcd['postalCode'] .
              "',cat:'" . $curRcd['name'] . "Unlimited'}";
       }
       echo '];';
       echo '</script>';
     }
   }
}

$fail = dbConnect($db_conn);
$ctstID = $_SESSION['ctstID'];
startHead("Contest Participant Map");
if ($fail == '') generate_marker_data($db_conn, $ctstID);
?>
<meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
    <style type="text/css">
      #map_canvas { height: 800px; width:1400px; }
    </style>
    <script type="text/javascript"
      src="http://maps.googleapis.com/maps/api/js?key=AIzaSyAoGeFLs6fqDgfOGVOwThfBqrKouS9TQfQ&sensor=false">
    </script>
    <script type="text/javascript">
      function displayMap() {
        var mapOptions = {
          center: new google.maps.LatLng(38.68,-96),
          zoom: 5,
          mapTypeId: google.maps.MapTypeId.ROADMAP
        };
        var map = new google.maps.Map(document.getElementById("map_canvas"),
            mapOptions);
        geocoder = new google.maps.Geocoder();
        records.forEach(function(record) {
          geocoder.geocode( { 'address': record.zip}, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
              var marker = new google.maps.Marker({
                map: map,
                position: results[0].geometry.location,
                title: record.cat
              });
            } 
          });
        });
      }
    </script>
<?php
startContent("onload='displayMap()'");
echo '<h1>Contest Participant Map</h1>';
echo '<a href="index.php">Back</a>';
echo '<div id="map_canvas"></div>';
endContent();
?>
