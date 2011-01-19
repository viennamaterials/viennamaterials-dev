#! /usr/bin/env python
#encoding: utf-8

import os
import os.path,glob,types,re,sys
import Configure,config_c,Options,Utils,Logs
from Logs import warn
from Configure import conf

# =============================================================================
def options(opt):
   opt.add_option('--viennautils-inc', type='string', help='path to the ViennaUtils include directory', default='', dest='viennautilsinc')
   opt.add_option('--viennautils-lib', type='string', help='path to the ViennaUtils library directory', default='', dest='viennautilslib')

# =============================================================================
def set_default(kw,var,val):
   if not var in kw:
      kw[var]=val

# =============================================================================
def validate_viennautils(self,kw):
   set_default(kw,'errmsg','not found')
   set_default(kw,'okmsg', 'ok')
   set_default(kw,'found_includes',False)

# =============================================================================
def find_includes_viennautils(self,kw):
   Path=getattr(Options.options,'viennautilsinc','')
   if Path == '':
      Path = '../ViennaUtils'

   VersionFile = Path + '/ViennaUtils/version.hh'

   if os.path.isfile(VersionFile):
      self.found_includes=1
      self.env['INCLUDES_VIENNAUTILS']=[Path,Path+'/external']
      return Path
   else:
      self.found_includes=0
      self.env['INCLUDES_VIENNAUTILS']=''
      return self.fatal('ViennaUtils headers not found!')

# =============================================================================
def find_libraries_viennautils(self,kw):
   Path=getattr(Options.options,'viennautilslib','')
   if Path == '':
      Path = '../ViennaUtils/lib'

   File = Path + '/libviennautils.a'

   if os.path.isfile(File):
      self.found_libraries=1
      self.env['LIBPATH_VIENNAUTILS']=[Path]
      self.env['LIB_VIENNAUTILS']='viennautils'
      return Path
   else:
      self.found_includes=0
      self.env['LIBPATH_VIENNAUTILS']=''
      self.env['LIB_VIENNAUTILS']=''
      return self.fatal('ViennaUtils libraries not found!')

# =============================================================================
def check_viennautils(self,*k,**kw):

   self.validate_viennautils(kw)
   ret=None
   try:
      if not kw.get('found_includes',None):
         self.start_msg(kw.get('msg_includes', 'Checking for ViennaUtils include path'))
         ret=self.find_includes_viennautils(kw)
   except Configure.ConfigurationError,e:
      if'errmsg'in kw:
         self.end_msg(kw['errmsg'], 'YELLOW')
      if'mandatory'in kw:
         if Logs.verbose>1:
            raise
         else:
            self.fatal('the configuration failed (see %r)'%self.log.name)
   else:
      if 'okmsg' in kw:
         self.end_msg(kw.get('okmsg_includes', ret))

   try:
      if not kw.get('found_libraries',None):
         self.start_msg(kw.get('msg_includes', 'Checking for ViennaUtils libraries'))
         ret=self.find_libraries_viennautils(kw)
   except Configure.ConfigurationError,e:
      if'errmsg'in kw:
         self.end_msg(kw['errmsg'], 'YELLOW')
      if'mandatory'in kw:
         if Logs.verbose>1:
            raise
         else:
            self.fatal('the configuration failed (see %r)'%self.log.name)
   else:
      if 'okmsg' in kw:
         self.end_msg(kw.get('okmsg_includes', ret))

   return ret

# =============================================================================
conf(validate_viennautils)
conf(find_includes_viennautils)
conf(find_libraries_viennautils)
conf(check_viennautils)

