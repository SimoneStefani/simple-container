FROM python:3.6-alpine

RUN apk update && apk add bash g++ make && rm -rf /var/cache/apk/*

WORKDIR /home

COPY . .
