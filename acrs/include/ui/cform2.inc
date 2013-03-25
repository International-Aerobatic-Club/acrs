<?php
//--------------------------------------------------------------------
//
// Author:      Peter M. Jensen
// Date:        23-May-2005
// Description:
//
//--------------------------------------------------------------------

require_once ('./fpdf/fpdf.php');

define ("TYPE_NONE",             0);
define ("TYPE_TEXT_LINE",        1);
define ("TYPE_TEXT_LINES",       2);
define ("TYPE_UNDERLINE",        3);
define ("TYPE_CHECKBOX",         4);
define ("TYPE_LINE",             5);
define ("TYPE_DOUBLE_LINE",      6);
define ("TYPE_IMAGE",            7);
define ("TYPE_XMARK",            8);
define ("TYPE_CIRCLE",           9);

define ("UNIT",                4.5);

function Enum_Image ($enum)
{
    switch ($enum) {
        case TYPE_NONE:            return "TYPE_NONE";
        case TYPE_TEXT_LINE:       return "TYPE_TEXT_LINE";
        case TYPE_TEXT_LINES:      return "TYPE_TEXT_LINES";
        case TYPE_TEXT_UNDERLINE : return "TYPE_UNDERLINE";
        case TYPE_CHECKBOX:        return "TYPE_CHECKBOX";
        case TYPE_LINE:            return "TYPE_LINE";
        case TYPE_DOUBLE_LINE:     return "TYPE_DOUBLE_LINE";
        case TYPE_IMAGE:           return "TYPE_IMAGE";
        case TYPE_XMARK:           return "TYPE_XMARK";
    }
}


function Parse_Font ($font)
{
    $fonts = explode ('-', $font);
    if (isset ($fonts [0])) {
        $font_type = $fonts [0];
    }
    else {
        $font_type = '';
    }
    if (isset ($fonts [1])) {
        $font_style = $fonts [1];
    }
    else {
        $font_style = '';
    }

    if ($font_style == 'Bold') {
        $font_style = 'B';
    }
    else if ($font_style == 'Oblique') {
        $font_style = 'I';
    }
    return array ($font_type, $font_style);
}

class CElement {
    
    var $elem_type;
    var $x;
    var $y;
    var $x_end;
    var $size;
    var $text;
    var $font_type;
    var $font_style;
    var $font_size;
    var $image_file;

    function CElement ($elem_type, $x, $y)
    {
         $this->elem_type = $elem_type;
         $this->x         = $x;
         $this->y         = $y;
    }

}

class CForm {

    var $pages;

    function CForm ()
    {
        $this->pages = array ();
    }

    function CForm_Add_Text_Line ($page, $x, $y, $font_type, $font_size, $text)
    {
        $elem = new CElement (TYPE_TEXT_LINE, $x, $y);
        $font = Parse_Font ($font_type);
        $elem->font_type  = $font [0];
        $elem->font_style = $font [1];
        $elem->font_size = $font_size;
        $elem->text      = $text;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Text_Lines ($page, $x, $y, $x_end, $font_type, $font_size, $text)
    {
        $elem = new CElement (TYPE_TEXT_LINES, $x, $y);
        $font = Parse_Font ($font_type);
        $elem->font_type  = $font [0];
        $elem->font_style = $font [1];
        $elem->x_end      = $x_end;
        $elem->font_size  = $font_size;
        $elem->text       = $text;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Underline ($page, $x, $y, $x_end, $font_size)
    {
        $elem = new CElement (TYPE_UNDERLINE, $x, $y);
        $elem->font_size   = $font_size;
        $elem->x_end       = $x_end;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Checkbox ($page, $x, $y, $size)
    {
        $elem = new CElement (TYPE_CHECKBOX, $x, $y);
        $elem->size = $size;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Xmark ($page, $x, $y, $size)
    {
        $elem = new CElement (TYPE_XMARK, $x, $y);
        $elem->size = $size;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Circle ($page, $x, $y, $size)
    {
        $elem = new CElement (TYPE_CIRCLE, $x, $y);
        $elem->size = $size;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Line ($page, $x, $y, $x_end)
    {
        $elem = new CElement (TYPE_LINE, $x, $y);
        $elem->x_end = $x_end;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Double_Line ($page, $x, $y, $x_end)
    {
        $elem = new CElement (TYPE_DOUBLE_LINE, $x, $y);
        $elem->x_end = $x_end;
        $this->pages [$page][] = $elem;
    }

    function CForm_Add_Image ($page, $x, $y, $x_end, $image_file)
    {
        $elem = new CElement (TYPE_IMAGE, $x, $y);
        $elem->image_file = $image_file;
        $elem->x_end = $x_end;
        $this->pages [$page][] = $elem;
    }

    function CForm_Make_Pdf ($width, $height, $scale, $to_browser = true)
    {
        // create new document
        
        $pdf = new FPDF ('P', 'pt', 'Letter');
        $pdf->SetAutoPageBreak (false);
        $pdf->SetDisplayMode ("fullpage", "single");
        $pdf->SetSubject ("IAC Contest Forms");

        $unit = $scale*UNIT;

        foreach ($this->pages as $page => $elements) {
            $pdf->AddPage();

            foreach ($elements as $elem) {
                switch ($elem->elem_type) {

                    case TYPE_TEXT_LINE:
                        $pdf->SetFont ($elem->font_type, $elem->font_style, $elem->font_size*$scale);
                        $pdf->SetXY ($elem->x*$unit, $elem->y*$unit);
                        $pdf->Cell (0, $elem->font_size*$scale, $elem->text);
                        break;

                    case TYPE_TEXT_LINES:
                        $pdf->SetFont ($elem->font_type, $elem->font_style, $elem->font_size*$scale);
                        $pdf->SetXY ($elem->x*$unit, $elem->y*$unit);
                        $pdf->MultiCell (($elem->x_end - $elem->x)*$unit,
                          $elem->font_size*$scale*1.2, $elem->text);
                        break;

                    case TYPE_UNDERLINE:
                        $pdf->SetLineWidth (1*$scale);
                        $y = $elem->y*$unit + $elem->font_size*$scale;
                        $pdf->Line ($elem->x*$unit, $y, $elem->x_end*$unit, $y);
                        break;

                    case TYPE_LINE:
                        $pdf->SetLineWidth (1*$scale);
                        $pdf->Line ($elem->x*$unit, $elem->y*$unit, $elem->x_end*$unit, $elem->y*$unit);
                        break;

                    case TYPE_DOUBLE_LINE:
                        $pdf->SetLineWidth (1*$scale);
                        $pdf->Line ($elem->x*$unit, $elem->y*$unit, $elem->x_end*$unit, $elem->y*$unit);
                        $pdf->Line ($elem->x*$unit, $elem->y*$unit + 2*$scale, $elem->x_end*$unit, $elem->y*$unit + 2*$scale);
                        break;

                    case TYPE_CHECKBOX:
                        $pdf->SetLineWidth (1*$scale);
                        $pdf->Rect ($elem->x*$unit, $elem->y*$unit, $elem->size*$scale, $elem->size*$scale);
                        break;

                    case TYPE_XMARK:
                        $pdf->SetLineWidth (1*$scale);
                        $pdf->Line ($elem->x*$unit, $elem->y*$unit, $elem->x*$unit + $elem->size*$scale, $elem->y*$unit + $elem->size*$scale);
                        $pdf->Line ($elem->x*$unit + $elem->size*$scale, $elem->y*$unit, $elem->x*$unit, $elem->y*$unit + $elem->size*$scale);
                        break;

                    case TYPE_CIRCLE:
                        $pdf->SetLineWidth (1*$scale);
                        $d3 = $elem->size/3;
                        $x  = $elem->x*$unit;
                        $y  = $elem->y*$unit;
                        $pdf->Line ($x + 1*$d3, $y,         $x + 2*$d3, $y);
                        $pdf->Line ($x + 2*$d3, $y,         $x + 3*$d3, $y + $d3);
                        $pdf->Line ($x + 3*$d3, $y + 1*$d3, $x + 3*$d3, $y + 2*$d3);
                        $pdf->Line ($x + 3*$d3, $y + 2*$d3, $x + 2*$d3, $y + 3*$d3);
                        $pdf->Line ($x + 2*$d3, $y + 3*$d3, $x + 1*$d3, $y + 3*$d3);
                        $pdf->Line ($x + 1*$d3, $y + 3*$d3, $x,         $y + 2*$d3);
                        $pdf->Line ($x,         $y + 2*$d3, $x,         $y + 1*$d3);
                        $pdf->Line ($x,         $y + 1*$d3, $x + 1*$d3, $y);
                        break;

                    case TYPE_IMAGE:
                        $pdf->Image ($elem->image_file, $elem->x*$unit, $elem->y*$unit, ($elem->x_end - $elem->x)*$unit);
                        break;
                }
            }

        }
    
        // output document  

        if ($to_browser) {
            $pdf->Output ();
        }
        else {
            return $pdf->Output ('', 'S');
        }

    }

    function CForm_Html_Dump ()
    {
        $html = "<HTML><BODY>";
        foreach ($this->pages as $page => $elements) {
            $html = $html . "<B>Page: $page</B><BR>\n";
            foreach ($elements as $elem) {
                $html = $html . $this->cform_Elem_Image ($elem) . "<BR>\n";
            }
        }
        $html = $html . "</BODY></HTML>";
        echo $html;
    }

    function cform_Elem_Image ($elem)
    {
        $html = "";
        $html = $html . "elem_type: "   . Enum_Image ($elem->elem_type) . "<BR>\n";
        $html = $html . "x: "           . $elem->x . "<BR>\n";
        $html = $html . "y: "           . $elem->y . "<BR>\n";
        $html = $html . "size: "        . $elem->size . "<BR>\n";
        $html = $html . "text: "        . $elem->text . "<BR>\n";
        $html = $html . "font_type: "   . $elem->font_type . "<BR>\n";
        $html = $html . "font_size: "   . $elem->font_size . "<BR>\n";
        $html = $html . "line_length: " . $elem->line_length . "<BR>\n";
        return $html;
    }

}
