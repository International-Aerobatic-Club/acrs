<?php

/**
 Generate an eight character string with random upper and lower-case
 Latin alphabetic characters or Arabic numerals.
 */
function generatePassword()
{
   $pwd = "";
   for($i = 1; $i <= 8; ++$i)
   {
      $c[0] = chr(rand(65, 90));
      $c[1] = chr(rand(97,122));
      $c[2] = chr(rand(48, 57));
      $idx = rand(0, 2);
      $pwd .= $c[$idx];
   }
   return $pwd;
}

/**
 * Take user entered password and return encoded password
 */
function encodePWD($password)
{
   $password = crop($password, 32);
   $pwd = sha1($password);
   debug('encodePWD '.$password.' is '.$pwd);
   return $pwd;
}

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
