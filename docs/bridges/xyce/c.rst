Xyce: C interface
#################

This is a simplified API based on `XyceCInterface <https://github.com/Xyce/Xyce/tree/master/utils/XyceCInterface>`_. It seems that does not support binding ``void** ptr`` from C to VHDL. Hence, unconstrained strings are used instead, which can be accesed as ``char** id`` from C. A storage mechanism is used to manage actual pointers associated to each id (string).

.. NOTE::
    Given the similarity between ``void**`` ``char**``, it might be possible to define a custom subtype of any type and use an access type to it as a placeholder to manage C void pointers in VHDL. To be investigated...

.. code-block:: c

    uint32_t xhdl_init(char** id, char** circuit);
    uint32_t xhdl_run(char** id, double requestedUntilTime, fat_t* tArray, fat_t* vArray);
    double xhdl_read(char** id, char** name);
    void xhdl_close(char** id);
