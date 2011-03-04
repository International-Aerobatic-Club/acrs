// Functions support practice slot reservations

var totalCount = 0;
var maxTotal = 1;
var sessionMax = new Array();
var sessionCount = new Array();

function initReservationTotal(maxCount, currentCount)
{
  maxTotal = maxCount;
  totalCount = currentCount;
}

function initReservationSession(sessionIndex, maxCount, currentCount)
{
   sessionMax[sessionIndex] = maxCount;
   sessionCount[sessionIndex] = currentCount;
}

function setDisplay(div, isShown)
{
   if(isShown)
   {  
      div.style.display="block";
   }
   else
   {
      div.style.display="none";
   }
}

function checkEnabledSubmit()
{
  var form = document.forms[0];
  var ttlOK = totalCount <= maxTotal;
  var ctsOK = true;
  for (var i in sessionCount)
  {
    ctsOK &= sessionCount[i] <= sessionMax[i];
  }
  form.submit.disabled = !(ctsOK && ttlOK);
  setDisplay(document.getElementById("ttlWarning"), !ttlOK);
  setDisplay(document.getElementById("ssnCtWarning"), !ctsOK);
}

// update count of reserved slots
function checkSlot(input, sessionIndex)
{
   if (input.checked)
   {
      totalCount += 1;
      sessionCount[sessionIndex] += 1;
   }
   else
   {
      totalCount -= 1;
      sessionCount[sessionIndex] -= 1;
   }
   checkEnabledSubmit();
}
