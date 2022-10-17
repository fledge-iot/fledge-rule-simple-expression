#!/usr/bin/env bash

BRANCH='develop'


if [ -d $HOME/fledge-service-notification ]
then
    echo "Updating fledge-service-notification Repository"
    cd $HOME/fledge-service-notification && git checkout $BRANCH && git branch && git pull && echo "Repository Updated at $(pwd)"
else
    echo "Cloning fledge-service-notification Repository"
    cd $HOME && git clone -b $BRANCH https://github.com/fledge-iot/fledge-service-notification.git && echo "Cloning Done at $(pwd)"
fi