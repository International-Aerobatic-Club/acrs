<?php
/*  siteLayout.inc, acrs/include/ui, dlco, 10/22/10
 *  general HTML page layout functions used for each page
 *
 *  Changes:
 *    02/17/2011 dcl        updated copyright years in the footer
 *    02/17/2011 dcl        image and stylesheet paths didn't work for me, so I inserted REGISTRATION_URL to make them absolute.
 *    10/22/10 jim_ward     use system-wide constants from include/config.inc.  Preface title with "ACRS".
 *                          Adjust image path for the IAC logo to work from acrs/ or admin/ directory.
 */

require_once ('dbConfig.inc');

function startHead($title)
{
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<meta content="en-us" http-equiv="Content-Language"/>
<meta content="text/html; charset=UTF8" http-equiv="Content-Type"/>
<meta http-equiv="Content-Style-Type" content="text/css"/>
<?php
    echo '<link href="'.REGISTRATION_URL.'iacRegistrationStyle.css" type="text/css" rel="stylesheet"/>';
    echo "<title>ACRS " . $title . "</title>";
}

function startContent($bodyAttributes = '')
{
    echo "</head><body " . $bodyAttributes . ">";
?>
<table cellspacing="0" cellpadding="0" class="layout">
<tbody>
<tr class="headline">
<td class="sideline">
<?php
  echo '<img alt="IAC Logo" src="'.REGISTRATION_URL.'logos/iac_logo_130.gif" border="0"/>';
?>
</td>
<td class="headline">
<?php
    $location = '';
    $dates = '';
    if (isset ($_SESSION['ctst_descr']))
    {
        $contest = $_SESSION['contest'];
        $location = strhtml($contest['location']);
        $dates = datehtml($contest['startDate'], '%A, %B %d') . ' to ' .
        datehtml($contest['endDate'], '%A, %B %d %Y');
        $title = $_SESSION['ctst_descr'];
    }
    else
    {
        $title = "Contest Registration";
    }
    echo '<h1 class="headline">' . $title . '</h1>';
    if ($location != '')
        echo '<h2 class="headline">' . $location . '</h2>';
    if ($dates != '')
        echo '<h2 class="headline">' . $dates . '</h2>';
?>
</td>
</tr></tbody></table>
<div class="content">
<?php


}

function endContent()
{
?>
</div>
<div class="footer">
<p>
      Copyright &copy; 2007 - 2011, International Aerobatic Club.
      All logos, trademarks, pictures, and videos are the property 
      of their respective owners.  Contact
<?php
      echo '<a href="mailto:' . ADMIN_EMAIL . '" class="footer">' . ADMIN_NAME . '</a>';
?>
      with comments or questions related to the display
      or content of this web page.
    </p>
    <p>This is an open source project supported by 
    <a href="http://sourceforge.net/projects/acrs" class="footer">SourceForge</a>.</p>
</div>
</body>
</html>
<?php


}
?>
<?php
/*
   Copyright 2008, 2011 International Aerobatic Club, Inc.

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
