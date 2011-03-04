// Functions support volunteer table

// Copy state of input to every cell in the table
function checkTable(input)
{
   var isChecked = input.checked;
   var td = input.parentNode;
   var tr = td.parentNode;
   var tbody = tr.parentNode;
   for (var i = 0; i < tbody.rows.length; ++i)
   {
      tr = tbody.rows[i];
      checkRowCells(tr, isChecked);
   }
}

// Set state of input of all cells in row
function checkRowCells(row, isChecked)
{
   // don't do the first column (headers)
   for (var i = 1; i < row.cells.length; ++i)
   {
      var td = row.cells[i];
      checkCell(td, isChecked);
   }
}

// Set state of any input to isChecked
function checkCell(cell, isChecked)
{
   for (var i = 0; i < cell.childNodes.length; ++i)
   {
      var child = cell.childNodes[i];
      if (child.nodeName == "INPUT")
      {
         child.checked = isChecked;
      }
   }
}

// Copy state of input to every cell in the same row of the table
function checkRow(input)
{
   var isChecked = input.checked;
   var td = input.parentNode;
   var tr = td.parentNode;
   checkRowCells(tr, isChecked);
}

// Copy state of input to every cell in the same column of the table
function checkColumn(input)
{
   var isChecked = input.checked;
   var td = input.parentNode;
   var col = td.cellIndex;
   var tr = td.parentNode;
   var tbody = tr.parentNode;
   for (var i = 0; i < tbody.rows.length; ++i)
   {
      var tr = tbody.rows[i];
      checkCell(tr.cells[col], isChecked);
   }
}

// clear check state of top cell in column and left cell in row
function uncheckRowColumn(input)
{
   var td = input.parentNode;
   var col = td.cellIndex;
   var tr = td.parentNode;
   checkCell(tr.cells[1], false);
   var tbody = tr.parentNode;
   tr = tbody.rows[0];
   checkCell(tr.cells[col], false);
   checkCell(tr.cells[1], false);
}
