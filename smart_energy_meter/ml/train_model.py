
#!/usr/bin/env python3
"""Offline training for appliance classifier."""

import sys, pathlib, joblib
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report

FEATURES = ['voltage','current','power','power_delta','voltage_delta','current_delta','power_mean']

def make_features(df):
    df = df.copy()
    df['power_delta']   = df['power'].diff().fillna(0)
    df['voltage_delta'] = df['voltage'].diff().fillna(0)
    df['current_delta'] = df['current'].diff().fillna(0)
    df['power_mean']    = df['power'].rolling(5, min_periods=1).mean()
    return df

def main(csv_path):
    df = pd.read_csv(csv_path)
    df = make_features(df)
    X = df[FEATURES]
    y = df['appliance']
    X_train, X_test, y_train, y_test = train_test_split(X, y, stratify=y, test_size=0.2, random_state=42)
    clf = RandomForestClassifier(n_estimators=200, class_weight='balanced', random_state=42)
    clf.fit(X_train, y_train)
    y_pred = clf.predict(X_test)
    print(classification_report(y_test, y_pred))
    out = pathlib.Path(__file__).with_name('model.joblib')
    joblib.dump({'model': clf, 'features': FEATURES}, out)
    print('Saved', out)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: python train_model.py data/dataset.csv'); sys.exit(1)
    main(sys.argv[1])
