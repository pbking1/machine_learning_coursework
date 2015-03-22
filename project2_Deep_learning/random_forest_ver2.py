from sklearn import ensemble
import pandas as pd
loc_train = "cifar-10-train.csv"
loc_test = "cifar-10-test.csv"
loc_sub = "submission-demo.csv"

df_train = pd.read_csv(loc_train)
df_test = pd.read_csv(loc_test)
feature_cols = [col for col in df_train.columns if col not in ['label']]

X_train = df_train[feature_cols]
X_test = df_test[feature_cols]
y = df_train['label']
test_ids = df_test['id']
clf = ensemble.RandomForestClassifier(n_estimators = 150, n_jobs = -1)
clf.fit(X_train, y);
with open(loc_sub, "wb") as outfile:
	outfile.write("id,label\n")
	for e, v in enumerate(list(clf.predict(X_test))):
		outfile.write("%s,%s\n" %(test_ids[e],v))
