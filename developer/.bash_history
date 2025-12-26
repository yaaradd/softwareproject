find ~/.vscode-server/extensions -type f -path "*/bin/cpptools" -exec chmod +x {} \;
find ~/.cursor-server/extensions -type f -path "*/bin/cpptools" -exec chmod +x {} \;
find ~/.vscode-server/extensions -type f -path "*/bin/OpenDebugAD7" -exec chmod +x {} \;
find ~/.cursor-server/extensions -type f -path "*/bin/OpenDebugAD7" -exec chmod +x {} \;
cd HW0
gcc -ansi -Wall -Wextra -Werror -pedantic-errors id1_id2_bc.c -lm -o bc
gcc -ansi -Wall -Wextra -Werror -pedantic-errors hw0.c -lm -o bc
gcc -ansi -Wall -Wextra -Werror -pedantic-errors hw0.c -lm -o bc
./bc
git init
git remote add origin https://github.com/yaaradd/softwareproject.git
git add .
git commit -m "Initial commit"
git config --global user.email "yaaradd@gmail.com"
  git config --global user.name "yaaradd"
git commit -m "Initial commit"
git branch -M main
git remote add origin https://github.com/yaaradd/softwareproject.git
git remote add origin https://github.com/yaaradd/softwareproject.gitdeveloper@221dd1ef5717:/home/HW0$ git remote add origin https://github.com/yaaradd/softwareproject.git
error: remote origin already exists.it push -u origin main
it push -u origin main
git push -u origin main
cd /home/HW0
./test_hw0.sh
git commit -m "hw0 tests"
git add .
git commit -m "hw0 tests"
git push
./test_hw0.sh
git add .
git commit -m "more tests"
git push
