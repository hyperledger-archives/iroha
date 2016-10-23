.. _devGuide:

.. highlight:: rst

==================================
Consensus events
==================================

This site covers Iroha's usage & API documentation. For basic info on what
Iroha is, please see `the main project website <http://iroha.tech>`_.


Transactions
--------

Though binary support is planned in the future, all transactions in Iroha are currently serialized as JSON.

.. code-block:: json
    :linenos:
    {
        "signature" : "[base64 sigature]"
    }



Membership
--------

Trust system (Hijiri)
--------

Line numbers are useful for long blocks such as this one:

.. code-block:: json
   :linenos:

   {
           "signature" : "[base64 sigature]"
       }
