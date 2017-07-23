#!/usr/bin/env bash

set -v


if [ "$CIRCLE_PULL_REQUEST" == false ] || [ -z "$CIRCLE_PULL_REQUEST" ]; then
  exit 0
fi


if [[ $CIRCLE_PULL_REQUEST =~ ([0-9]*)$ ]]; then
  PR_NUMBER=${BASH_REMATCH[1]}
fi


# depencies
apt-get update -y && apt-get install -y lcov npm nodejs-legacy
npm install -g lcov-summary

cd $IROHA_BUILD 
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.info
lcov --remove coverage.info '*external*' --output-file coverage.info
RESULT_COVERAGE=`lcov-summary ./coverage.info | sed -z 's/\n/\\\\n/g'`

curl -XPOST -H 'Content-Type:application/json' -H "Authorization: token $GITHUB_TOKEN" -d "{\"body\":\"$RESULT_COVERAGE\"}" https://api.github.com/repos/$CIRCLE_PROJECT_USERNAME/$CIRCLE_PROJECT_REPONAME/issues/$PR_NUMBER/comments
