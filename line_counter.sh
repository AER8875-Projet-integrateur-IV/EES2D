#!/bin/bash
git config --local alias.count "! git ls-files | grep -Ev 'txt|su2|vtu|LICENSE|ees2d|.gitignore|.clang-format|.dat|.travis.yml' | xargs wc -l"
git count