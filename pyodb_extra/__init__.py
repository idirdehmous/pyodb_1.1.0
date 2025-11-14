# -*- coding: utf-8 -*- 
import logging
logging.basicConfig(level=logging.INFO)
log = logging.getLogger(__name__)



def main():
    """Entry point for the application script"""
    print("Call your main application code here")



__version__ = "_1.1.0"

from  .parser        import *
from  .odb_ob        import *
from  .odb_glossary  import *
from  .exceptions    import *
from  .environment   import *
from  .dr_hook       import * 


