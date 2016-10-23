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

.. code-block:: postgresql
   :linenos:

   -- http://www.postgresonline.com/journal/index.php?/archives/97-SQL-Coding-Standards-To-Each-His-Own-Part-II.html
   SELECT persons.id, persons.first_name, persons.last_name, forums.category,
      COUNT(DISTINCT posts.id) as num_posts,
      COALESCE(MAX(comments.rating), 0) AS highest_rating,
      COALESCE(MIN(comments.rating), 0) AS lowest_rating
   FROM persons JOIN posts ON persons.id = posts.author
      JOIN forums on posts.forum = forums.id
      LEFT OUTER JOIN comments ON posts.id = comments.post
   WHERE persons.status > 0
      AND forums.ratings = TRUE
      AND comments.post_date > ( now() - INTERVAL '1 year')
   GROUP BY persons.id, persons.first_name, persons.last_name, forums.category
   HAVING count(DISTINCT posts.id) > 0
   ORDER BY persons.last_name, persons.first_name; 
