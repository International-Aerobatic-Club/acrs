<?php
/*  useful.inc, acrs/acrs/include, dlco, 10/23/2010
 *  common functions
 *
 *  Changes:
 *    10/23/2010 jim_ward       sense fopen() failure and, if so, don't try to fwrite() or fclose() to log files;
 *                              also, use DEBUG_ACTION, EMAIL_ACTION and ADMIN_EMAIL
 */

  require_once('dbConfig.inc');
  require_once('data/encodeHTML.inc');

  function isDebug()
  {
    return DEBUG;
  }
  
  /**
  Display each element of an array, one line per element.
  */
  function debugArr($message, $arr)
  {
     debug($message);
     if (isDebug())
     {
       if (is_array($arr)) foreach ($arr as $key => $value)
       {
          debug($key.':'.$value.':', false);
       }
       else
       {
          debug('non array value is '.$arr, false);
       }
     }
  }

  /**
  Check debug, act based on DEBUG_ACTION
  */
  function debug($message, $doTimestamp=true)
  {
     if (isDebug())
     {
        if (DEBUG_ACTION == "echo")
        {
           echo "<p>".$message."</p>\n";
        }
        else if (DEBUG_ACTION == "mail")
        {
           $fromAddress = "From: ".ADMIN_EMAIL."\r\n";
           mail(ADMIN_EMAIL, "debug output", $message, $fromAddress);
        } 
        else
        {
           if ($log = fopen(DEBUG_ACTION, 'a'))
	   {
              if ($doTimestamp) fwrite($log, date('Y-m-d:H:i:s')."\n");
              fwrite($log, $message."\n");
              fclose($log);
           }   /* fopen() worked */
        }   /* action = write to log file */
     }   /* debug mode */
  }

  /**
  Check EMAIL_ACTION, send mail if not, otherwise echo mail.
  email: comma separated list of recipient addresses
  subject: appears as the email subject
  mailContest: appears as the email contest
  headers: email headers terminated with \r\n must include From: header.
  */
  function do_email($email, $subject, $mailContent, $headers)
  {
     if (EMAIL_ACTION == "echo")
     {
        echo "<p>Mail<ul>\n";
        echo "<li>Mail to:".$email."</li>\n";
        echo "<li>Subject:".$subject."</li>\n";
        echo "<li>Content:".$mailContent."</li>\n";
        echo "<li>Headers:".$headers."</li>\n";
        echo "</ul></p>\n";
     }
     else if (EMAIL_ACTION == "mail")
     {
        mail($email, $subject, $mailContent, $headers);
     }
     else
     {
        if ($log = fopen(EMAIL_ACTION, 'a'))
        {
           fwrite($log, date('Y-m-d:H:i:s')."\n");
           fwrite($log, "Mail to:".$email."\n");
           fwrite($log, "Subject:".$subject."\n");
           fwrite($log, "Headers:".$headers."\n");
           fwrite($log, "Content:".$mailContent."\n");
           fclose($log);
	}   /* fopen() worked */
     }   /* append email to log file */
  }

  /**
  Handle bad errors. Log them or mail them to the administrator.
  message gets repeated in the HTML error content.
  message and place get repeated in the administrator message.
  Return HTML content about the error.
  */
  function notifyError($message, $context)
  {
     $subject = "script error";
     $mailContent = "Script error in ".$context." is ".$message.".\n";
     if (isset($_SESSION["userID"]))
     {
        $mailContent .= "Encountered by registrant ".$_SESSION["userID"];
     }
     $fromAddress = "From: ".ADMIN_EMAIL."\r\n";
     do_email(ADMIN_EMAIL, $subject, $mailContent, $fromAddress);
     return "Internal error: ".
        strhtml($message).". Mail sent to adminstrator.";
  }

  /**
  Validate an email address.
  Provide email address (clean input)
  Returns true if the email address has the email address format
  and the domain exists.
  */
  function validEmail($email)
  {
     $isValid = true;
	 $atIndex = strrpos($email, "@");
	 if (is_bool($atIndex) && !$atIndex)
	 {
	    $isValid = false;
     }
	 else
	 {
	    $domain = substr($email, $atIndex+1);
		$local = substr($email, 0, $atIndex);
		$localLen = strlen($local);
		$domainLen = strlen($domain);
        if ($localLen < 1 || $localLen > 64)
        {
           // local part length exceeded
           $isValid = false;
        }
        else if ($domainLen < 1 || $domainLen > 255)
        {
           // domain part length exceeded
           $isValid = false;
        }
        else if ($local[0] == '.' || $local[$localLen-1] == '.')
        {
           // local part starts or ends with '.'
           $isValid = false;
        }
        else if (preg_match('/\\.\\./', $local))
        {
           // local part has two consecutive dots
           $isValid = false;
        }  
        else if (!preg_match('/^[A-Za-z0-9\\-\\.]+$/', $domain))
        {
           // character not valid in domain part
           $isValid = false;
        }
        else if (preg_match('/\\.\\./', $domain))
        {
           // domain part has two consecutive dots
           $isValid = false;
        }  
		else if (!preg_match('/^(\\\\.|[A-Za-z0-9!#%&`_=\\/$\'*+?^{}|~.-])+$/', str_replace("\\\\","",$local)))
        {
           // character not valid in local part unless local part is quoted
           if (!preg_match('/^"(\\\\"|[^"])+"$/', str_replace("\\\\","",$local)))
           {
              $isValid = false;
           }
        }

        /*  Caution!  I've made a small semantic change here.  Prior to my 10/26/10 change, this code checked for
         *  "isset($debug)", which was always set and therefore never checked the domain's MX or A record.  This code
         *  checks the actual DEBUG mode and suppresses the domain MX/A record check only when DEBUG is set to true.
         *  This may yield different behavior than existed before.  -Jim W.
         */
        if ($isValid && !(DEBUG || checkdnsrr($domain,"MX") || checkdnsrr($domain, "A")))
        {
           // domain not found in DNS
           $isValid = false;
        }
     }
     return $isValid;
  }
?>
<?php
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
