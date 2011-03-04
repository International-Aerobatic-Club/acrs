<?php
set_include_path('./include');
require_once ("useful.inc");
require_once ("ui/siteLayout.inc");
require_once ("data/encodeHTML.inc");

startHead("Text test.  Use to check special character handling");
startContent();
$text = '';
if (isset($_POST["send"]))
{
   $text = $_POST['text'];
   debug ('Text content is ' . $text);
   echo '<p>Text content: ';
   echo strhtml($text);
   echo '</p>';
}
echo '<form method="post">'.
'<input name="text" value="'.strhtml($text).'"'.
	'type="text"/>'.
	'<input name="send" type="submit" />'.
	'</form>';
endContent();
/*
 Copyright 2010 International Aerobatic Club, Inc.

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
