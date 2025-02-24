#!/bin/bash

# loop thorugh the directory
# only access files that end in .jpg
# then run the command, and have the test file have the same name as the 
# picture used, plus the translation done

echo "RUNNING TESTS"

directory="$1"

for file in "$directory"/*; 
do
        extension=$(echo "$file" | awk -F. '{print $NF}')
        if [ "$extension" == "jpg" ]
        then
                echo "## NAME: $(basename "$file")" > $(basename "$file").out
                file_size=$(stat -c %s $file)
                echo "## SIZE: $file_size bytes" >> $(basename "$file").out

                echo "$file"

                method="-row-major"

                echo "### ROW MAJOR ###" >> $(basename "$file").out
                eval "djpeg ${file} | ./ppmtrans ${method} -time timeBash.out > bashTest.out"
                echo "rotate 0 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 90 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 90 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 270 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 270 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 180 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 180 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -flip horizontal ${method} -time timeBash.out > bashTest.out"
                echo "horizontal ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -flip vertical ${method} -time timeBash.out > bashTest.out"
                echo "vertical ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -transpose ${method} -time timeBash.out > bashTest.out"
                echo "transpose ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out
                echo "" >> $(basename "$file").out

                
                
                echo "COL MAJOR" >> $(basename "$file").out 
                method="-col-major"

                eval "djpeg ${file} | ./ppmtrans ${method} -time timeBash.out > bashTest.out"
                echo "rotate 0 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 90 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 90 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 270 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 270 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 180 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 180 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -flip horizontal ${method} -time timeBash.out > bashTest.out"
                echo "horizontal ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -flip vertical ${method} -time timeBash.out > bashTest.out"
                echo "vertical ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -transpose ${method} -time timeBash.out > bashTest.out"
                echo "transpose ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out
                echo "" >> $(basename "$file").out

                echo "BLOCK MAJOR" >> $(basename "$file").out 
                method="-block-major"

                eval "djpeg ${file} | ./ppmtrans ${method} -time timeBash.out > bashTest.out"
                echo "rotate 0 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 90 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 90 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 270 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 270 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -rotate 180 ${method} -time timeBash.out > bashTest.out"
                echo "rotate 180 ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -flip horizontal ${method} -time timeBash.out > bashTest.out"
                echo "horizontal ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -flip vertical ${method} -time timeBash.out > bashTest.out"
                echo "vertical ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out

                eval "djpeg ${file} | ./ppmtrans -transpose ${method} -time timeBash.out > bashTest.out"
                echo "transpose ${method}" >> $(basename "$file").out
                eval "cat timeBash.out >> $(basename "$file").out"
                echo "" >> $(basename "$file").out
                echo "" >> $(basename "$file").out
        fi
done

echo "TESTS DONE"