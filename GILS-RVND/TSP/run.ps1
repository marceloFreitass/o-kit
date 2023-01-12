Set-ExecutionPolicy Unrestricted
$OUTPUT =  "results.txt"

$files = Get-ChildItem "instances"
foreach($f in $files){
    
    "$f" >> $OUTPUT
    "$f"
& ".\tsp.exe"  ".\instances\$f">> $OUTPUT
"" >> $OUTPUT

}


 

