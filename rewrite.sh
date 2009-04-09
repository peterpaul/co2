#!/bin/bash

./rewrite_method_call.awk $1 > $1~
mv $1~ $1
