cc_library(name = 'term',
           srcs = [
                   'term.cpp',
                  ],
          )

cc_library(name = 'term_spool',
           srcs = [
                   'term_spool.cpp',
                  ],
           deps = [
                   ':term',
                  ],
          )

cc_library(name = 'article_spool',
           srcs = [
                   'article_spool.cpp',
                   'article.cpp',
                   'heap_util.cpp',
                  ],
           deps = [
                   ':term',
                   ':term_spool',
                   '//innotree/hotnews/sort/extractTerms:count_word',
                  ],
          )

cc_binary(name = 'main',
          srcs = ['main.cpp'],
          deps = [
                  ':article_spool',
                 ],
         )

