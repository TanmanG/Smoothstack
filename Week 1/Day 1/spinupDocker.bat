@ECHO OFF

docker run -it --privileged --mount src=C:\Docker\Smoothstack,target=/Docker,type=bind dockerdev bash

PAUSE