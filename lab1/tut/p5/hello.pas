program hello(output);

procedure box (s: char; w, h: integer);
    var i,j:integer;
    begin
        for i := 1 .. h do begin
            for j := 1 to w do
                write(s);
            writeln
        end;
    end;
 
begin

    box ('+', 10, 1);

    box ('$', 20, 1);
 
end.
