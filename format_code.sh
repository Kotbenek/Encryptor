# Format the code
clang-format --style=file -i src/*
# Add newline at end of file if missing
find . -type f -not -path './.git/*' -not -path './bin/*' -not -path './obj/*' -not -path './documents/*' -not -path './format_code.sh' -exec sed -i -e '$a\' {} \;
# Remove extra newlines if there is more than one at the end of file
find . -type f -not -path './.git/*' -not -path './bin/*' -not -path './obj/*' -not -path './documents/*' -not -path './format_code.sh' -exec sed -i -e :a -e '/^\n*$/{$d;N;ba' -e '}' {} \;
# Remove trailing spaces from code
find ./src/* -type f -exec sed -i 's/[[:space:]]\+$//' {} \;
