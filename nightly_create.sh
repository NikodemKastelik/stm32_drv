#!/bin/bash

FILENAME="branches_to_merge.txt";

BRANCHES=$(cat $FILENAME);

git checkout nightly
if [ $? -ne 0 ]; then { echo "Could not change branch. Aborting." ; exit 1; } fi

git rev-parse origin/master | awk '{cmd="git reset --hard "; print cmd $1}' | bash

for branch in $BRANCHES
do
    git rebase origin/"$branch"
done
git checkout -
