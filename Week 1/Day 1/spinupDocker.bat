@ECHO OFF

docker run -it --privileged --mount src=I:\Docker\Smoothstack,target=/Docker,type=bind dockerdev bash

PAUSE