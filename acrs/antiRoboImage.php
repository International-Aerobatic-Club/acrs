<?php
/*  antiRoboImage.php, acrs/admin, dlco, 10/23/2010
 *  display Captcha image
 *
 *  Changes:
 *    10/23/2010 jim_ward       be sensitive to presence of FreeType support; if absent, imagettftext() doesn't exist
 *                              so use GDF font & functions instead.
 */

session_start();
set_include_path('./include');
require("dbConfig.inc");
require("securimage.inc");
require("useful.inc");

$gd_info = gd_info();
$img = new securimage();

/*  If no FreeType support is available, don't call the undefined imagettftext() function from securimage::show().
 */
if (! $gd_info["FreeType Support"])
   $img->use_gd_font = true ;

$img->show("antiRoboImage.jpg");
debug("antiRobotCheat: " . $img->code);
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
