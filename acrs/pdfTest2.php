<?php
require_once ('./make_form2.php');

$form = new CForm();

Make_Entry_Template2($form, $pageOffset +1);
Make_Tech_Template($form, $pageOffset +3);
Make_Blank($form, $pageOffset +4);
Make_Order_Of_Flight_Template($form, $pageOffset +5);
Make_Volunteer_Template($form, $pageOffset +5);
Make_Blank($form, $pageOffset +6);

Output_Form($form, true);
?>
