###Recommandation system
- Problem formulation
    - 可能会有99%的数据是不知道的（用问号表示）
    - 因此可以去猜那些不知道的数据是多少
    - 然后把得分最高的电影推荐给他就可以了
    - 所以其实是一个矩阵补全系统

- content based recommender system
    - 假设有一些别的特征例如电影的浪漫成都或者动作成分含量（由专家弄出来的）
    - 因此可以把原有的评分作为输入，把专家的预测作为y
        - 做线性回归， 把每个用户的theta学出来
        - 然后把theta和用户的评分做内积，得出那些问号的数据
        
