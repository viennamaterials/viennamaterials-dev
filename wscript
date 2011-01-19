#! /usr/bin/env python
# encoding: utf-8

VERSION='0.0.1'
APPNAME='ViennaMaterials'   

top = '.'
out = 'build'

localtoolsdir = 'waftools/'

import sys

def options(ctx):
   ctx.load('g++')
   ctx.load('viennautils',   tooldir=localtoolsdir)    

def configure(ctx):
   ctx.load('g++')
   ctx.check_viennautils()    

   ctx.env.append_value('CXXFLAGS', ['-O2', '-fPIC', '-Wall', '-pipe'])


def build(ctx):
   #
   # build the examples
   # 
   #ctx.recurse('examples') 


   ctx(
      features     = 'cxx cxxprogram', 
      source       = 'examples/ex1.cpp',
      target       = 'ex1',
      includes     = '.',
      uselib       ='VIENNAUTILS'
   )


