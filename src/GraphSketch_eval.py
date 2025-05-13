import numpy as np
from sklearn.metrics import (
    roc_auc_score, precision_score, recall_score,
    precision_recall_curve, roc_curve, average_precision_score
)

def load_scores_and_labels(score_file, label_file):
    scores, labels = [], []
    with open(score_file, 'r') as sf, open(label_file, 'r') as lf:
        for s_line, l_line in zip(sf, lf):
            try:
                score = float(s_line.strip().split(",")[-1])
                label = int(l_line.strip())
                scores.append(score)
                labels.append(label)
            except:
                continue
    return np.array(scores), np.array(labels)

def evaluate(score_file, label_file):
    scores, labels = load_scores_and_labels(score_file, label_file)

    auc = roc_auc_score(labels, scores)
    avg_precision = average_precision_score(labels, scores)

    # Save ROC + PR curve
    fpr, tpr, thresholds = roc_curve(labels, scores)
    with open("roc_data.txt", "w") as f:
        f.write("FPR\tTPR\tThreshold\n")
        for fp, tp, thr in zip(fpr, tpr, thresholds):
            f.write(f"{fp:.6f}\t{tp:.6f}\t{thr:.6f}\n")

    precisions, recalls, pr_thresholds = precision_recall_curve(labels, scores)
    with open("pr_data.txt", "w") as f:
        f.write("Recall\tPrecision\tThreshold\n")
        for p, r, thr in zip(precisions, recalls, np.append(pr_thresholds, 1.0)):
            f.write(f"{r:.6f}\t{p:.6f}\t{thr:.6f}\n")

    # Multi-K Evaluation
    k_values = [50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
    print("\nEvaluation Results")
    print("===========================")
    print(f"AUC-ROC: {auc:.4f}")
    print("-" * 60)

    for K in k_values:
        top_k_indices = np.argsort(scores)[-K:]
        top_k_labels = labels[top_k_indices]

        # If no positive predictions, handle warning gracefully
        precision_at_k = precision_score(np.ones_like(top_k_labels), top_k_labels, zero_division=0)
        recall_at_k = recall_score(np.ones_like(top_k_labels), top_k_labels, zero_division=0)

        print(f"Precision@{K:<4}: {precision_at_k:.4f}  ||  Recall@{K:<4}: {recall_at_k:.4f}")

    print("-" * 60)
    print(f"Average Precision (AP): {avg_precision:.4f}")
    print("===========================")
    print("ROC curve data saved to 'roc_data.txt'.")
    print("Precision-Recall curve data saved to 'pr_data.txt'.")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python GraphSketch_eval.py score.txt labels.txt")
        sys.exit(1)

    score_file = sys.argv[1]
    label_file = sys.argv[2]
    evaluate(score_file, label_file)
