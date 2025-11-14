Usage
=====

Example
-------

.. code-block:: python

   from pyodb import odbDict

   data = odbDict(
       database="body.odb",
       sql_query="SELECT * FROM body",
       fcols=0,
       fmt_float=3
   )
