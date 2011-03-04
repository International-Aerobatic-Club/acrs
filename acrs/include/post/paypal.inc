<?php
/*  paypal.inc, acrs/include, dlco, 10/26/2010
 *  publish the paypal registration button and component fields
 *
 *  Changes:
 *      10/26/2010 jim_ward     change cancel button text to reflect the contest name, rather than "Nationals".
 *                              Our submit button now says, "Pay your registration fee".  Add trailing slashes
 *                              before XML/HTML close angle-bracket for consistency.
 *      10/24/2010 jim_ward     use PAYPAL constants, rather than globals.
 */

require_once ('data/encodePOST.inc');

function regularRegistrationButton($registrationFee, $registrant, $contest)
{
    $registrationType = $contest['name'] . ' ' . $contest['regYear'];
    $registrationType .= ': ' . $registrant['name']; // is the category name
    if (boolChecked($registrant, 'isStudent'))
        $registrationType .= '/student';
    if (boolChecked($registrant, 'teamAspirant'))
        $registrationType .= '/team aspirant';
    if (boolChecked($registrant, 'fourMinFree'))
        $registrationType .= '/four minute free';

    echo "<form action='" . PAYPAL_SERVER_URL . "' method='post'>\n";

    echo "<input type='hidden' name='cmd' value='_xclick'/>\n";
    echo "<input type='hidden' name='no_shipping' value='1'/>\n";
    echo "<input type='hidden' name='no_note' value='1'/>\n";
    echo "<input type='hidden' name='rm' value='1'/>\n";

    echo "<input type='hidden' name='return' value='" . PAYPAL_RETURN_URL . "'/>\n";
    echo "<input type='hidden' name='cancel_return' value='" . PAYPAL_CANCEL_URL . "'/>\n";
    echo "<input type='hidden' name='notify_url' value='" . PAYPAL_NOTIFY_URL . "'/>\n";
    echo "<input type='hidden' name='business' value='" . $contest['payEmail'] . "'/>\n";
    echo "<input type='hidden' name='custom' value='" . inthtml($registrant['regID']) . "'/>\n";
    echo "<input type='hidden' name='amount' value='" . $registrationFee . "'/>\n";
    echo "<input type='hidden' name='currency_code' value='USD'/>\n";
    echo "<input type='hidden' name='item_name' value='" . $registrationType . "'/>\n";
    echo "<input type='hidden' name='item_number' value='IAC:" . $registrant['iacID'] . "'/>\n";
    echo "<input type='hidden' name='image_url' value='http://www.usnationalaerobatics.org/USN2007/logos/NationalsLogoPP.jpg'/>\n";
    echo "<input type='hidden' name='first_name' value='" . $registrant['givenName'] . "'/>\n";
    echo "<input type='hidden' name='last_name' value='" . $registrant['familyName'] . "'/>\n";
    echo '<input type="hidden" name="lc" value="US" />' . "\n";
    echo '<input type="hidden" name="cbt" value="Return to ' . $contest['name'] . ' registration"' . " />\n";
    //echo '<input type="hidden" name="bn" value="PP-BuyNowBF">';
?>
<input type='submit' name='submit' value='Pay your registration fee'/>

</form>
<?php

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
